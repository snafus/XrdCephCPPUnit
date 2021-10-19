#ifndef RADOSCONNECT_HPP
#define RADOSCONNECT_HPP

#include <rados/librados.h>
#include <radosstriper/libradosstriper.h>
#include <radosstriper/libradosstriper.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <string>

class Cluster {
    public:
        void connect();
        void disconnect();
        rados_striper_t & striper() {return m_striper;}
        rados_ioctx_t   & ioctx()   {return io;}
        ~Cluster();

        int lock_soid(const std::string& path);
        int rmlock   (const std::string& path, const std::string & xattr_name="lock.striper.lock");

    protected:
        std::string getUUID();

    private:
        const int STRIPER_NUM_STRIPES = 1;
        const int STRIPER_STRIPE_UNIT = 8388608;
        const int STRIPER_OBJECT_SIZE = 67108864;

        rados_t cluster;
        rados_ioctx_t io;
        const std::string     poolname = "dteam";
        const std::string cluster_name = "ceph";
        const std::string    user_name = "client.xrootd";
        uint64_t flags;
        rados_striper_t m_striper;

};

#endif