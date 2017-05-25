cmake_minimum_required (VERSION 2.8.7)
project (mcc)
set(ROOT_DIR ../../)
set(TEST_LIB_DIR ${ROOT_DIR}/test/)
include_directories(${ROOT_DIR}/)

#-Wall -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -std=c++11

# Build to Unit Test
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weffc++ -g -Wall -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/local/include/json-c -std=c++11 -pthread")

# Build to meassure Coverage
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -O0 -Weffc++ -g -Wall -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/local/include/json-c -std=c++11 -pthread")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin/")

add_executable(
    test
    ${TEST_LIB_DIR}/testSuite.cpp
    ${TEST_LIB_DIR}/testOption.cpp
    ${TEST_LIB_DIR}/configTest/tester.cpp
    ${TEST_LIB_DIR}/configTest/testmain.cpp
    ${TEST_LIB_DIR}/configTest/configOptTestSuite.cpp
    ${ROOT_DIR}/conf/confData.cpp
    ${ROOT_DIR}/conf/configurator.cpp
)

# -ljson-c -lglib-2.0
target_link_libraries(test json-c glib-2.0)
