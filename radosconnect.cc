#include "radosconnect.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#define EXIT_FAILURE 1

Cluster::~Cluster() {
    //disconnect();
}

void Cluster::connect() {

        int err;
        err = rados_create2(&cluster, cluster_name.c_str(), user_name.c_str(), flags);
        if (err < 0)
        {
                fprintf(stderr, "%s: Couldn't create the cluster handle! %s\n", "JW", strerror(-err));
                exit(EXIT_FAILURE);
        }
        err = rados_conf_read_file(cluster, "/etc/ceph/ceph.conf");
        if (err < 0)
        {
                fprintf(stderr, "%s: cannot read config file: %s\n", "JW", strerror(-err));
                exit(EXIT_FAILURE);
        }
        // err = rados_conf_parse_argv(cluster, argc, argv);
        // if (err < 0)
        // {
        //         fprintf(stderr, "%s: cannot parse command line arguments: %s\n", "JW", strerror(-err));
        //         exit(EXIT_FAILURE);
        // }
        err = rados_connect(cluster);
        if (err < 0)
        {
                fprintf(stderr, "%s: cannot connect to cluster: %s\n", "JW", strerror(-err));
                exit(EXIT_FAILURE);
        }
        err = rados_ioctx_create(cluster, poolname.c_str(), &io);
        if (err < 0)
        {
                fprintf(stderr, "%s: cannot open rados pool %s: %s\n", "JW", poolname, strerror(-err));
                rados_shutdown(cluster);
                exit(EXIT_FAILURE);
        }

        err = rados_striper_create(io, &m_striper);
        if (err < 0)
        {
                fprintf(stderr, "Unable to create RADOS striper object for pool %s: ERR=%s\n", poolname, strerror(-err));
                rados_shutdown(cluster);
                exit(EXIT_FAILURE);
        }

        err = rados_striper_set_object_layout_stripe_unit(m_striper, STRIPER_STRIPE_UNIT);
        if (err < 0)
        {
                fprintf(stderr, "Unable to set RADOS striper unit size to %d  for pool %s: ERR=%s\n", STRIPER_STRIPE_UNIT, poolname, err);
                rados_shutdown(cluster);
                exit(EXIT_FAILURE);
        }

        err = rados_striper_set_object_layout_stripe_count(m_striper, STRIPER_NUM_STRIPES);
        if (err < 0)
        {
                fprintf(stderr, "Unable to set RADOS striper stripe count to %d  for pool %s: ERR=%s\n", STRIPER_NUM_STRIPES, poolname, err);
                rados_shutdown(cluster);
                exit(EXIT_FAILURE);
        }

        err = rados_striper_set_object_layout_object_size(m_striper, STRIPER_OBJECT_SIZE);
        if (err < 0)
        {
                fprintf(stderr, "Unable to set RADOS striper stripe count to %d  for pool %s: ERR=%s\n", STRIPER_OBJECT_SIZE, poolname, err);
                rados_shutdown(cluster);
                exit(EXIT_FAILURE);
        }
    return;

}

void Cluster::disconnect() {
    rados_ioctx_destroy(io);
    rados_shutdown(cluster);
}


int Cluster::lock_soid(const std::string& path) {

        librados::IoCtx ctx;
        librados::IoCtx::from_rados_ioctx_t(ioctx(), ctx);

        std::string lockCookie = getUUID();
        int rc = ctx.lock_exclusive(path.c_str(), "striper.lock", lockCookie, "", 0, 0);
        // if (rc < 0) {
        //         fprintf(stderr, "Lock error");
        //         rados_shutdown(cluster);
        //         exit(EXIT_FAILURE);
        // }
        return rc;
}

int Cluster::rmlock   (const std::string& path, const std::string & xattr_name) {
        int rc = rados_striper_rmxattr(striper(),
				(path+".0000000000000000").c_str(),
				xattr_name.c_str()
                                );
        return rc;
}


std::string Cluster::getUUID()
{
        // very basic uuid-like generator
         /* initialize random seed: */
        srand (time(NULL));

        const char *chars = "0123456789abcdef";
        // b5355e20-16a4-4fe8-b14f-befd60319f66
        char suuid[37];
        for (int i=0; i < 37; ++i) {
                if (i == 36) {suuid[i] = '\0';}
                else if (i == 8)  {suuid[i] = '-';}
                else if (i == 13)  {suuid[i] = '-';}
                else if (i == 18)  {suuid[i] = '-';}
                else if (i == 23)  {suuid[i] = '-';}
                else {
                        suuid[i] = chars[rand()%16];
                }
        }
        return std::string(suuid);

        // Ceph implementation using boost
        //   struct uuid_d uuid;
        //   uuid.generate_random();
        //   char suuid[37];
        //   uuid.print(suuid);
        //   return std::string(suuid);

}