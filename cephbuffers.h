// CppUnit testing for XrdCephPosix library
// file: wrappertest.h
// Author: Sarah Byrne
#ifndef cephbuffers_H
#define cephbuffers_H


#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>





class cephbuffers : public CPPUNIT_NS :: TestFixture
{
    CPPUNIT_TEST_SUITE (cephbuffers);
    CPPUNIT_TEST (readIObuffer);
    CPPUNIT_TEST (readAIObuffer);
    // CPPUNIT_TEST (openTest);
    // CPPUNIT_TEST (statTest);
    // CPPUNIT_TEST (readTest);
    CPPUNIT_TEST_SUITE_END ();

    public:
    void setUp    (void);
    void tearDown (void);


    protected:
        void readIObuffer (void);
        void readAIObuffer (void);

    private:

};



#endif