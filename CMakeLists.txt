cmake_minimum_required (VERSION 2.8.7)
project (mcc)

#-Wall -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -std=c++11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/local/include/json-c -std=c++11 -pthread")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin/")
set(ROOT_DIR ./)

add_executable(
    mchubd
    ${ROOT_DIR}/src/manager.cpp
    ${ROOT_DIR}/src/mediator.cpp
    ${ROOT_DIR}/src/connectionInfo.cpp
    ${ROOT_DIR}/src/contract.cpp
    ${ROOT_DIR}/src/messageHandler.cpp
    ${ROOT_DIR}/src/channelStatusMediator.cpp
    ${ROOT_DIR}/src/registerClientHandler.cpp
    ${ROOT_DIR}/src/message.cpp
    ${ROOT_DIR}/src/clientManager.cpp
    ${ROOT_DIR}/src/channelManager.cpp
    ${ROOT_DIR}/src/taskSet.cpp
)

# -ljson-c -lglib-2.0
target_link_libraries(mchubd json-c glib-2.0)
