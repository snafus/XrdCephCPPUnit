#include "locktest.h"

#include <XrdCephPosix.hh>
#include <XrdOuc/XrdOucEnv.hh>
#include <fcntl.h>
#include <sys/stat.h>

#include <sstream>

using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION (locktest);


void locktest :: setUp (void)
{
    TestFixture::setUp();
    // ensure an existing file
    struct stat buf, *bufp = &buf;
    int rc = ceph_posix_stat((XrdOucEnv*)NULL, "dteam:test/jwalder/testfile", bufp);
    CPPUNIT_ASSERT_EQUAL(rc,0);

    
}

void locktest :: tearDown (void)
{
    //remove the file from ceph
    TestFixture::tearDown();
}


void locktest::noopTest() {
    CPPUNIT_ASSERT_EQUAL(0,0);
}
