// CppUnit testing for XrdCephPosix library
// file: wrappertest.h
// Author: Sarah Byrne
#ifndef extentstest_H
#define extentstest_H


#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>





class extentstest : public CPPUNIT_NS :: TestFixture
{
    CPPUNIT_TEST_SUITE (extentstest);
    CPPUNIT_TEST (testOverlap);
    // CPPUNIT_TEST (openTest);
    // CPPUNIT_TEST (statTest);
    // CPPUNIT_TEST (readTest);
    CPPUNIT_TEST_SUITE_END ();

    public:
    void setUp    (void);
    void tearDown (void);


    protected:
        void testOverlap (void);

    private:

};



#endif