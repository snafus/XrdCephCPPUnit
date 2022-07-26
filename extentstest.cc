// CppUnit testing for XrdCephPosix library
// file: wrappertest.cc
// Author: J. Walder (modified from work of Sarah Byrne)
#include "extentstest.h"

#include <XrdCephPosix.hh>
#include <XrdOuc/XrdOucEnv.hh>
#include <fcntl.h>
#include <sys/stat.h>

#include "XrdCephBuffers/BufferUtils.hh"
// #include "XrdCephBuffers/XrdCephBufferDataSimple.hh"
// #include "XrdCephBuffers/CephIOAdapterRaw.hh"
// #include "XrdCephBuffers/CephIOAdapterAIORaw.hh"

#include <sstream>
#include <memory>

#include "zlib.h"

using namespace std;


CPPUNIT_TEST_SUITE_REGISTRATION (extentstest);

void extentstest :: setUp (void)
{
    TestFixture::setUp();
}

void extentstest :: tearDown (void)
{
    // ceph_posix_disconnect_all();
    TestFixture::tearDown();
}

void extentstest::testOverlap (void)
{
    using namespace XrdCephBuffer;
    Extent response(0,0);

    response = Extent(10,10).containedExtent(Extent(10,10));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Full overlap (a==b) off ", off_t(10), response.offset()); 
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Full overlap (a==b) len", size_t(10), response.len()); 

    response = Extent(5,10).containedExtent(Extent(10,10));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Partial overlap (a<=b) off ", off_t(10), response.offset()); 
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Partial overlap (a=>b) len", size_t(5), response.len()); 

    response = Extent(10,10).containedExtent(Extent(5,10));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Partial overlap (a<=b) off ", off_t(10), response.offset()); 
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Partial overlap (a=>b) len", size_t(5), response.len()); 

    response = Extent(7,5).containedExtent(Extent(5,10));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Subset (a in b) off ", off_t(7), response.offset()); 
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Subset (a in b) len", size_t(5), response.len()); 

    response = Extent(5,10).containedExtent(Extent(7,5));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Subset (a in b) off ", off_t(7), response.offset()); 
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Subset (a in b) len", size_t(5), response.len()); 

    response = Extent(0,5).containedExtent(Extent(10,5));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("NonOverlap (a<b) off ", off_t(0), response.offset()); 
    CPPUNIT_ASSERT_EQUAL_MESSAGE("NonOverlap (a<b) len", size_t(0), response.len()); 

    response = Extent(10,5).containedExtent(Extent(0,5));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("NonOverlap (a>b) off ", off_t(0), response.offset()); 
    CPPUNIT_ASSERT_EQUAL_MESSAGE("NonOverlap (a>b) len", size_t(0), response.len()); 

    response = Extent(5,5).containedExtent(Extent(10,5));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Touching (a<b) off ", off_t(10), response.offset()); 
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Touching (a<b) len", size_t(0), response.len()); 

    response = Extent(10,5).containedExtent(Extent(5,5));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Touching (a>b) off ", off_t(10), response.offset()); 
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Touching (a>b) len", size_t(0), response.len()); 

}
