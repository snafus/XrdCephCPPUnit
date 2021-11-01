
cmake_minimum_required(VERSION 3.13)

project(cmake_testapp)
set(CMAKE_CXX_STANDARD 14)

find_library(CPPUNIT cppunit)

include_directories(/usr/include/xrootd)

#include_directories(/home/tier1/jwalder/Projects/XrdCeph_CppUnit/FirstTests/xrootd-ceph/src/XrdCeph)
include_directories(/xrootd/xrootd-ceph/src/XrdCeph)
#link_directories(/root/jwalder/xrdcephtests/cppunit/InitialTests/XRD_CEPH_BUILD/src)
link_directories(/xrootd/xrootd-ceph/build/src)



#ADD_LIBRARY( locktestLib STATIC locktest.cc )

# cephposixtest.cc
add_executable(test RunTest.cpp clusterconnecttest.cc locktest.cc radosconnect.cc)
target_link_libraries(test XrdCephPosix cppunit rados radosstriper)
