#include "messageTransportService.h"
#include "connectionInfo.h"
#include "connection/socket.h"
#include "connection/udpServer.h"

#if FEAT_TEST_MODE
#include "../src/test/testStub.h"
#endif

mcHubd::MessageTransportService::MessageTransportService(){}
mcHubd::MessageTransportService::~MessageTransportService(){}

void mcHubd::MessageTransportService::sendAll(std::string message)
{
#if FEAT_TEST_MODE
    mcHubd::TestStub::getInstance()->addSubscribeMsg(message); //Test Code
#else
    /*
    std::map<std::string, struct sockaddr_in> pool = mcHubd::ConnectionInfo::getInstance()->getUDPConnPool();
    std::map<std::string, struct sockaddr_in>::iterator itor;

    itor = pool.begin();
    while(itor != pool.end())
    {
        struct sockaddr_in clientAddr = itor->second;
        std::cout << __PRETTY_FUNCTION__ << "[" << __LINE__ << "] message: " << message << std::endl;

        mcHubd::MessageTransportService::udpsend(clientAddr, message);
        ++itor;
        sleep(1);
    }
    */
#endif
}

void mcHubd::MessageTransportService::sendto(std::string message, mcHubd::Message* msg)
{
#if FEAT_TEST_MODE
    TestStub::getInstance()->addRespMsg(message); //Test Code
#else

    if(msg)
    {
        struct sockaddr_in clientAddr = msg->getSockAddr();;
        std::cout << __PRETTY_FUNCTION__ << "[" << __LINE__ << "] message: " << message << std::endl;

        mcHubd::MessageTransportService::udpsend(clientAddr, message);
    }
#endif
}


bool mcHubd::MessageTransportService::udpsend(struct sockaddr_in targetaddr, std::string message)
{
#if FEAT_TEST_MODE
    return true;
#else
    mcHubd::Socket* udpServer = mcHubd::ConnectionInfo::getInstance()->getServer();

    if(udpServer)
    {
        int serverfd = udpServer->getSockfd();
        std::cout << __PRETTY_FUNCTION__ << "[" << __LINE__ << "] fd: " << serverfd << std::endl;
        if(serverfd != -1)
            return mcHubd::UDPServer::sendmsg(serverfd, targetaddr, message);
    }

    return false;
#endif
}
