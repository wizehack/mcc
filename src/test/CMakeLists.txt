cmake_minimum_required (VERSION 2.8.7)
project (mcc)

#-Wall -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -std=c++11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/local/include/json-c -std=c++11 -pthread")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin/")
set(ROOT_DIR ./)

add_executable(
    test
    ${ROOT_DIR}/testSuite.cpp
    ${ROOT_DIR}/testOption.cpp
    ${ROOT_DIR}/tester.cpp
    ${ROOT_DIR}/testmain.cpp
    ${ROOT_DIR}/baseFrameworkTestSuite.cpp
    ${ROOT_DIR}/../../src/manager.cpp
    ${ROOT_DIR}/../../src/mediator.cpp
    ${ROOT_DIR}/../../src/connectionInfo.cpp
    ${ROOT_DIR}/../../src/taskSet.cpp
)

# -ljson-c -lglib-2.0
target_link_libraries(test json-c glib-2.0)
