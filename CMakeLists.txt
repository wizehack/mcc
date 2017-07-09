cmake_minimum_required (VERSION 2.8.7)
project (mcc)
set(ROOT_DIR ./)
include_directories(${ROOT_DIR}/)
include_directories(${ROOT_DIR}/src)
#-Wall -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -std=c++11

# Build to Test
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weffc++ -O0 -g -Wall -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/local/include/json-c -std=c++11 -pthread")

# Build to meassure Coverage
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -O0 -Weffc++ -g -Wall -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/local/include/json-c -std=c++11 -pthread")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin/")

add_executable(
    mcc
    ${ROOT_DIR}/src/main.cpp
    ${ROOT_DIR}/src/conf/confData.cpp
    ${ROOT_DIR}/src/conf/configurator.cpp
    ${ROOT_DIR}/src/connection/connectionManager.cpp
    ${ROOT_DIR}/src/connection/socket.cpp
    ${ROOT_DIR}/src/connection/udpServer.cpp
    ${ROOT_DIR}/src/connection/socketData.cpp
    ${ROOT_DIR}/src/feature/manager.cpp
    ${ROOT_DIR}/src/feature/mediator.cpp
    ${ROOT_DIR}/src/feature/connectionInfo.cpp
    ${ROOT_DIR}/src/feature/contract.cpp
    ${ROOT_DIR}/src/feature/messageHandler.cpp
    ${ROOT_DIR}/src/feature/channelStatusMediator.cpp
    ${ROOT_DIR}/src/feature/registerClientHandler.cpp
    ${ROOT_DIR}/src/feature/registerChannelHandler.cpp
    ${ROOT_DIR}/src/feature/deleteChannelHandler.cpp
    ${ROOT_DIR}/src/feature/deleteClientHandler.cpp
    ${ROOT_DIR}/src/feature/requestChannelHandler.cpp
    ${ROOT_DIR}/src/feature/message.cpp
    ${ROOT_DIR}/src/feature/clientManager.cpp
    ${ROOT_DIR}/src/feature/channelManager.cpp
    ${ROOT_DIR}/src/feature/taskSet.cpp
    ${ROOT_DIR}/src/feature/messageTransportService.cpp
    ${ROOT_DIR}/src/feature/messageProcessor.cpp
)

# -ljson-c -lglib-2.0
target_link_libraries(mcc json-c glib-2.0)
