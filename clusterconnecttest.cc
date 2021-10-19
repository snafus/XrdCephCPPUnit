#include "clusterconnecttest.h"
#include <radosstriper/libradosstriper.hpp>

using namespace std;


CPPUNIT_TEST_SUITE_REGISTRATION (clusterconnecttest);


void clusterconnecttest :: setUp (void)
{
    TestFixture::setUp();
    m_cluster.connect();
}

void clusterconnecttest :: tearDown (void)
{
    TestFixture::tearDown();
    //int rc = rados_striper_remove(m_cluster.striper(), name.c_str() );   
    m_cluster.disconnect(); 
}


void clusterconnecttest :: connectTest (void)
{
    rados_pool_stat_t stats;

    int rc = rados_ioctx_pool_stat(m_cluster.ioctx(), &stats);

    CPPUNIT_ASSERT_EQUAL(0, rc);

}

void clusterconnecttest ::  writeTest()
{
    connectTest();
    char buf[] = "hello";
    size_t count = 6;
    off_t offset =0;
    //rados_striper_create()
    //int rc = m_cluster.striper().write(name.c_str(), bl, count, offset);
    int rc = rados_striper_remove(m_cluster.striper(), name.c_str() );   
    if (rc == -EBUSY) {
        rc = m_cluster.rmlock(name.c_str());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("removal lock failed", 0, rc); 
        rc = rados_striper_remove(m_cluster.striper(), name.c_str() );  
    }
    CPPUNIT_ASSERT_EQUAL_MESSAGE("removal failed", 0, rc); 
    rc = rados_striper_write(m_cluster.striper(), name.c_str(), buf, count, offset );
    CPPUNIT_ASSERT_EQUAL_MESSAGE("write failed", 0, rc); 
}

void clusterconnecttest ::  readTest()
{
    //CPPUNIT_ASSERT_EQUAL(0, rc);
    //int rc = rados_striper_write(m_cluster.striper(), name.c_str(), buf, count, offset );
    char buf[24];
    size_t count = 6;
    off_t offset =0;
    
    int rc = rados_striper_read(m_cluster.striper(), name.c_str(), buf, count, offset);
    //cout << "Buf: " << buf << " " << rc << "___" << endl;

    CPPUNIT_ASSERT(rc>=0);
}

void clusterconnecttest ::  removeTest()
{
    int rc = -1;
    //rc = m_cluster.lock_soid((name+".0000000000000000")); 
    //CPPUNIT_ASSERT_EQUAL_MESSAGE("lock failed", 0, rc); 
    rc = rados_striper_remove(m_cluster.striper(), name.c_str() );    
    CPPUNIT_ASSERT_EQUAL_MESSAGE("removal failed", 0, rc); 
}
