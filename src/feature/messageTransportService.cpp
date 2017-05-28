#include "messageTransportService.h"
#include "connectionInfo.h"

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
    std::map<int, std::shared_ptr<mcHubd::Connection>> pool = mcHubd::ConnectionInfo::getInstance()->getConnectionPool();
    std::map<int, std::shared_ptr<mcHubd::Connection>>::iterator itor;

    itor = pool.begin();

    std::cout << __PRETTY_FUNCTION__ << "[" << __LINE__ << "] pool size: " << pool.size() << std::endl;
    while(itor != pool.end())
    {
        int error = 0;
        socklen_t len = sizeof (error);
        int sockfd = itor->second->getSockfd();
        int socketState = getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &len);

        if(socketState == 0)
        {
            char buf[MAX_BUF_SIZE] = {0,};
            memset(buf, '\0', MAX_BUF_SIZE);
            (char*)memcpy(buf, message.c_str(), MAX_BUF_SIZE);

            std::cout << __PRETTY_FUNCTION__ << "[" << __LINE__ << "] send msg" << buf << std::endl;
            int len = write(sockfd, buf, MAX_BUF_SIZE);
            std::cout << __PRETTY_FUNCTION__ << "[" << __LINE__ << "] sent sockfd: " << sockfd << " len: " << len << std::endl;
            if (len <= 0)
                mcHubd::ConnectionInfo::getInstance()->closeConnection(sockfd);
        }
        else
        {
            std::cout << __PRETTY_FUNCTION__ << "[" << __LINE__ << "] sock opt error: " << sockfd << " socketState: " << socketState << " error no: " << error << std::endl;
            mcHubd::ConnectionInfo::getInstance()->closeConnection(sockfd);
        }

        ++itor;
    }
#endif
}

void mcHubd::MessageTransportService::sendto(std::string message, mcHubd::Message* msg)
{
#if FEAT_TEST_MODE
    TestStub::getInstance()->addRespMsg(message); //Test Code
#else
    int sockfd = msg->getSockfd();
    int err = 0;

    err = write(sockfd, message.c_str(), message.length());

    if (err < 0)
        std::cout << "can not send to client: " << sockfd << std::endl;
#endif
}
