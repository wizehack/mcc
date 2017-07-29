#include "messageTransportService.h"
#include "connectionInfo.h"
#include "connection/socket.h"
#include "connection/udpServer.h"
#include "channelStatusMediator.h"
#include "clientManager.h"
#include "channelManager.h"

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
    std::map<std::string, struct sockaddr_in> pool = mcHubd::ConnectionInfo::getInstance()->getUDPConnPool();
    std::map<std::string, struct sockaddr_in>::iterator itor;

    itor = pool.begin();
    while(itor != pool.end())
    {
        struct sockaddr_in clientAddr = itor->second;
        std::cout << __PRETTY_FUNCTION__ << "[" << __LINE__ << "] message: " << message << std::endl;

        if(mcHubd::MessageTransportService::udpsend(clientAddr, message) == false)
        {
            //delete client
            std::shared_ptr<mcHubd::ClientManager> clientMgr;
            std::shared_ptr<mcHubd::ChannelManager> channelMgr;
            std::string psName = itor->first;
            std::map<std::string, pid_t> pspidmap = mcHubd::ConnectionInfo::getInstance()->getConnectedClientKeyMap();
            pid_t pid = mcHubd::ConnectionInfo::getInstance()->getPID(psName);
            mcHubd::Mediator* mediator = new ChannelStatusMediator();
            mcHubd::Contract* contract = new mcHubd::Contract();

            clientMgr = std::make_shared<mcHubd::ClientManager>(mediator);
            channelMgr = std::make_shared<mcHubd::ChannelManager>(mediator);

            mediator->appendManager(clientMgr);
            mediator->appendManager(channelMgr);

            contract->setProcessName(psName);
            contract->setProcessId(pid);

            std::cout << __PRETTY_FUNCTION__ << "[" << __LINE__ << "] delete: " << psName << " pid: " << pid << std::endl;
            mediator->deleteClient(&contract);
            mcHubd::RESPCODE code = contract->getRespCode();

            if(code == MCHUBD_OK)
            {
                mcHubd::ConnectionInfo::getInstance()->deleteConnInfo(psName);
            }
            else
            {
                std::cout << __PRETTY_FUNCTION__ << "[" << __LINE__ << "] failed " << std::endl;
            }
        }
        ++itor;
        sleep(1);
    }
#endif
}

bool mcHubd::MessageTransportService::sendto(std::string message, mcHubd::Message* msg)
{
#if FEAT_TEST_MODE
    TestStub::getInstance()->addRespMsg(message); //Test Code
    return true;
#else

    if(msg)
    {
        struct sockaddr_in clientAddr = msg->getSockAddr();
        std::cout << __PRETTY_FUNCTION__ << "[" << __LINE__ << "] message: " << message << std::endl;
        return mcHubd::MessageTransportService::udpsend(clientAddr, message);
    }

    return false;
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
