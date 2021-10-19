#ifndef LOCKTEST_H
#define LOCKTEST_H


#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>





class locktest : public CPPUNIT_NS :: TestFixture
{
    CPPUNIT_TEST_SUITE (locktest);
    CPPUNIT_TEST (noopTest);
    // CPPUNIT_TEST (statTest);
    // CPPUNIT_TEST (readTest);
    CPPUNIT_TEST_SUITE_END ();

    public:
    void setUp    (void);
    void tearDown (void);


    protected:
        void noopTest();
        // void openTest (void);
        // void statTest (void);
        // void readTest (void);

    private:
        // int *a;

};



#endif
