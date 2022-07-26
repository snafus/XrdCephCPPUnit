// CppUnit testing for XrdCephPosix library
// file: wrappertest.cc
// Author: J. Walder (modified from work of Sarah Byrne)
#include "cephbuffers.h"

#include <XrdCephPosix.hh>
#include <XrdOuc/XrdOucEnv.hh>
#include <fcntl.h>
#include <sys/stat.h>

#include "XrdCephBuffers/BufferUtils.hh"
#include "XrdCephBuffers/XrdCephBufferDataSimple.hh"
#include "XrdCephBuffers/CephIOAdapterRaw.hh"
#include "XrdCephBuffers/CephIOAdapterAIORaw.hh"

#include <sstream>
#include <memory>

#include "zlib.h"

using namespace std;


CPPUNIT_TEST_SUITE_REGISTRATION (cephbuffers);

void cephbuffers :: setUp (void)
{
    TestFixture::setUp();
    // [user@]pool[,nbStripes[,stripeUnit[,objectSize]]]
    ceph_posix_set_defaults("xrootd@dteam,1,8388608,67108864");
}

void cephbuffers :: tearDown (void)
{
    // ceph_posix_disconnect_all();
    TestFixture::tearDown();
}

void cephbuffers::readIObuffer (void)
{

    std::unique_ptr<XrdCephBuffer::IXrdCephBufferData> buffer = 
        std::unique_ptr<XrdCephBuffer::IXrdCephBufferData>(new XrdCephBuffer::XrdCephBufferDataSimple(1024*1024));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Buffer Capacity wrong", size_t(1024*1024), buffer->capacity());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Buffer length wrong", size_t(0), buffer->length());

    int fd1 = ceph_posix_open((XrdOucEnv *)NULL, "dteam:test/jwalder/unittests/test1M", 0, (mode_t)NULL);
    CPPUNIT_ASSERT(fd1 >= 0);

    std::unique_ptr<XrdCephBuffer::ICephIOAdapter> ioalg = 
        std::unique_ptr<XrdCephBuffer::ICephIOAdapter>(new XrdCephBuffer::CephIOAdapterRaw(buffer.get(), fd1));

    ssize_t rc = ioalg->read(0, 16*1024*1024);
    CPPUNIT_ASSERT_MESSAGE("Read error", rc >=0);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Read length wrong", size_t(1024*1024), buffer->length());

    void *bufferp = (void *)malloc(1024*1024);
    for (int i=0; i < 1024*1024; ++i) ((char*)bufferp)[i] = 0;

    rc = buffer->readBuffer(bufferp,0, rc);
    CPPUNIT_ASSERT_MESSAGE("Buffer error", rc >=0);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("rc 1MiB", long(1024*1024), rc);
    long adl = adler32(1, (const unsigned char*)bufferp, rc); 
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Adler32 error", long(15728641), adl);

    ceph_posix_close(fd1);
}

void cephbuffers::readAIObuffer (void)
{

    std::unique_ptr<XrdCephBuffer::IXrdCephBufferData> buffer = 
        std::unique_ptr<XrdCephBuffer::IXrdCephBufferData>(new XrdCephBuffer::XrdCephBufferDataSimple(1024*1024));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Buffer Capacity wrong", size_t(1024*1024), buffer->capacity());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Buffer length wrong", size_t(0), buffer->length());

    int fd1 = ceph_posix_open((XrdOucEnv *)NULL, "dteam:test/jwalder/unittests/test1M", 0, (mode_t)NULL);
    CPPUNIT_ASSERT(fd1 >= 0);

    std::unique_ptr<XrdCephBuffer::ICephIOAdapter> ioalg = 
        std::unique_ptr<XrdCephBuffer::ICephIOAdapter>(new XrdCephBuffer::CephIOAdapterAIORaw(buffer.get(), fd1));

    ssize_t rc = ioalg->read(0, 16*1024*1024);
    CPPUNIT_ASSERT_MESSAGE("Read error", rc >=0);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Read length wrong", size_t(1024*1024), buffer->length());

    void *bufferp = (void *)malloc(1024*1024);
    for (int i=0; i < 1024*1024; ++i) ((char*)bufferp)[i] = 0;

    rc = buffer->readBuffer(bufferp,0, rc);
    CPPUNIT_ASSERT_MESSAGE("Buffer error", rc >=0);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("rc 1MiB", long(1024*1024), rc);
    long adl = adler32(1, (const unsigned char*)bufferp, rc); 
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Adler32 error", long(15728641), adl);
    
    ceph_posix_close(fd1);
}