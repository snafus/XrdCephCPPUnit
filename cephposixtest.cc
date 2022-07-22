// CppUnit testing for XrdCephPosix library
// file: wrappertest.cc
// Author: J. Walder (modified from work of Sarah Byrne)
#include "cephposixtest.h"

#include <XrdCephPosix.hh>
#include <XrdOuc/XrdOucEnv.hh>
#include <fcntl.h>
#include <sys/stat.h>

#include <sstream>

using namespace std;


CPPUNIT_TEST_SUITE_REGISTRATION (cephposixtest);

void cephposixtest :: setUp (void)
{
    TestFixture::setUp();
    // [user@]pool[,nbStripes[,stripeUnit[,objectSize]]]
    ceph_posix_set_defaults("xrootd@dteam,1,8388608,67108864");
}

void cephposixtest :: tearDown (void)
{
    ceph_posix_disconnect_all();
    TestFixture::tearDown();
}

void cephposixtest :: openTest (void)
{
    int fd1 = ceph_posix_open((XrdOucEnv *)NULL, "dteam:test/jwalder/unittests/test1M", 0, (mode_t)NULL);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("test1M not opened", 0, fd1);
    int fd2 = ceph_posix_open((XrdOucEnv *)NULL, "dteam:test/jwalder/unittests/helloworld", 0, (mode_t)NULL);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("helloworld not opened", 1, fd2);
    int fd3 = ceph_posix_open((XrdOucEnv *)NULL, "dteam:test/jwalder/thisisnotafile", 0, (mode_t)NULL);
    CPPUNIT_ASSERT(fd3 < 0);
}

void cephposixtest :: statTest (void)
{
    struct stat buf, *bufp = &buf;
    int rc1 = ceph_posix_stat((XrdOucEnv*)NULL, "dteam:test/jwalder/unittests/test1M", bufp);
    CPPUNIT_ASSERT_EQUAL(1048576l, buf.st_size);
    int rc2 = ceph_posix_stat((XrdOucEnv*)NULL, "dteam:test/jwalder/unittests/helloworld", bufp);
    CPPUNIT_ASSERT_EQUAL(12l, buf.st_size);

}

void cephposixtest :: readTest (void)
{
    int fd1 = ceph_posix_open((XrdOucEnv *)NULL, "dteam:test/jwalder/unittests/helloworld", 0, (mode_t)NULL);
    void *bufferp = (void *)malloc(1024);
    for (int i=0; i < 1024; ++i) ((char*)bufferp)[i] = 0;
    ssize_t a = ceph_posix_read(fd1, bufferp, 1024);
    CPPUNIT_ASSERT_MESSAGE("Failed read", a >=0);
    stringstream ss((char *)bufferp);
    string firstline;
    getline(ss, firstline, '\n');
    string expectedline = "Hello World";
    string message = "First line of file incorrect";
    CPPUNIT_ASSERT_EQUAL_MESSAGE(message, expectedline, firstline);
}