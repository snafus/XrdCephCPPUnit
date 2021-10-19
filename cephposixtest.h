// CppUnit testing for XrdCephPosix library
// file: wrappertest.h
// Author: Sarah Byrne
#ifndef CEPHPOSIXTEST_H
#define CEPHPOSIXTEST_H


#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>





class cephposixtest : public CPPUNIT_NS :: TestFixture
{
    CPPUNIT_TEST_SUITE (cephposixtest);
    CPPUNIT_TEST (openTest);
    CPPUNIT_TEST (statTest);
    CPPUNIT_TEST (readTest);
    CPPUNIT_TEST_SUITE_END ();

    public:
    void setUp    (void);
    void tearDown (void);


    protected:
        void openTest (void);
        void statTest (void);
        void readTest (void);

    private:
        int *a;

};



#endif