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
    ${TEST_LIB_DIR}/featureTest/tester.cpp
    ${TEST_LIB_DIR}/featureTest/testmain.cpp
    ${TEST_LIB_DIR}/featureTest/baseFrameworkTestSuite.cpp
    ${TEST_LIB_DIR}/featureTest/registerClientTestSuite.cpp
    ${TEST_LIB_DIR}/featureTest/registerChannelTestSuite.cpp
    ${TEST_LIB_DIR}/featureTest/deleteChannelTestSuite.cpp
    ${TEST_LIB_DIR}/featureTest/deleteClientTestSuite.cpp
    ${TEST_LIB_DIR}/featureTest/messageQueueTestSuite.cpp
    ${TEST_LIB_DIR}/featureTest/subscriptionTestSuite.cpp
    ${TEST_LIB_DIR}/featureTest/requestChannelTestSuite.cpp
    ${ROOT_DIR}/feature/testStub.cpp
    ${ROOT_DIR}/feature/manager.cpp
    ${ROOT_DIR}/feature/mediator.cpp
    ${ROOT_DIR}/feature/connectionInfo.cpp
    ${ROOT_DIR}/feature/contract.cpp
    ${ROOT_DIR}/feature/messageHandler.cpp
    ${ROOT_DIR}/feature/channelStatusMediator.cpp
    ${ROOT_DIR}/feature/registerClientHandler.cpp
    ${ROOT_DIR}/feature/registerChannelHandler.cpp
    ${ROOT_DIR}/feature/deleteChannelHandler.cpp
    ${ROOT_DIR}/feature/deleteClientHandler.cpp
    ${ROOT_DIR}/feature/requestChannelHandler.cpp
    ${ROOT_DIR}/feature/message.cpp
    ${ROOT_DIR}/feature/clientManager.cpp
    ${ROOT_DIR}/feature/channelManager.cpp
    ${ROOT_DIR}/feature/taskSet.cpp
    ${ROOT_DIR}/feature/messageQueue.cpp
    ${ROOT_DIR}/feature/messageTransportService.cpp
)

# -ljson-c -lglib-2.0
target_link_libraries(test json-c glib-2.0)
