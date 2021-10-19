#ifndef CLUSTERCONNECTTEST_HPP
#define CLUSTERCONNECTTEST_HPP


#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "radosconnect.h"



class clusterconnecttest : public CPPUNIT_NS :: TestFixture
{
    CPPUNIT_TEST_SUITE (clusterconnecttest);
    CPPUNIT_TEST (connectTest);
    CPPUNIT_TEST (writeTest);
    CPPUNIT_TEST (readTest);
    CPPUNIT_TEST (removeTest);

    // CPPUNIT_TEST (statTest);
    // CPPUNIT_TEST (readTest);
    CPPUNIT_TEST_SUITE_END ();

    public:
    void setUp    (void);
    void tearDown (void);


    protected:
        void connectTest();
        void writeTest();
        void readTest();
        void removeTest();
    
        // void openTest (void);
        // void statTest (void);
        // void readTest (void);

    private:
        // int *a;
        Cluster m_cluster;

        std::string name = "test/wtest";

};

#endif
