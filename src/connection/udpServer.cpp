#include "mcHubType.h"
#include "udpServer.h"

mcHubd::UDPServer::UDPServer(int port) : mcHubd::Socket(port)
{
    struct sockaddr_in serveraddr;

    this->m_sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(this->m_port);

    this->m_serveraddr = serveraddr;
}

mcHubd::UDPServer::~UDPServer(){}

bool mcHubd::UDPServer::start()
{
    int sockfd = this->m_sockfd;
    int binderr = 0;
    int optval = 1;

    unsigned int servlen = sizeof(this->m_serveraddr);

    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval) != 0)
    {
        std::perror("[SO_REUSEADDR] Error! ");
        return false;
    }

    binderr = bind(sockfd, (struct sockaddr *)&(this->m_serveraddr), servlen);

    if (binderr == -1)
    {
        std::perror("bind error : ");
        return false;
    }

    std::cout << __PRETTY_FUNCTION__ << " [" << __LINE__ << "] udp sockfd: "<< this->m_sockfd << std::endl;
    return true;
}

struct sockaddr_in mcHubd::UDPServer::getSockAddr() const
{
    return this->m_serveraddr;
}

bool mcHubd::UDPServer::sendmsg(int serverfd, struct sockaddr_in targetaddr, std::string msg)
{
    unsigned int clientaddrlen = sizeof(targetaddr);
    char buf[MAX_BUF_SIZE];
    char ackbuf[MAX_BUF_SIZE];
    int sentsize = 0;
    int recvsize = 0;
    bool ret = false;

    if(mcHubd::UDPServer::setTimeoutOpt(serverfd) == false)
    {
        mcHubd::UDPServer::rollbackTimeoutOpt(serverfd);
        return ret;
    }

    memset(buf, '\0', MAX_BUF_SIZE);
    memset(ackbuf, '\0', MAX_BUF_SIZE);
    (char*)memcpy(buf, msg.c_str(), MAX_BUF_SIZE-1);

    sentsize = sendto(serverfd, static_cast<void*>(buf), sizeof(buf), 0,
            (struct sockaddr*)&targetaddr, clientaddrlen);

    std::cout << __PRETTY_FUNCTION__ << "[" << __LINE__ << "] addr: " << targetaddr.sin_addr.s_addr << std::endl;
    if( sentsize != sizeof(buf) )
    {
        std::perror("[SEND] Error");
        std::cout << __PRETTY_FUNCTION__ << "[" << __LINE__ << "] " << "send error" << std::endl;
        std::cout << __PRETTY_FUNCTION__ << "[" << __LINE__ << "] " << "sentsize: " << sentsize << std::endl;

        if(mcHubd::UDPServer::rollbackTimeoutOpt(serverfd) == false)
            std::cout << __PRETTY_FUNCTION__ << "[" << __LINE__ << "] " << "Error" << std::endl;

        return ret;
    }

    recvsize = recvfrom(serverfd, (void *)&ackbuf, sizeof(ackbuf), 0,
            (struct sockaddr *)&targetaddr, &clientaddrlen);

    if( recvsize > 0)
    {
        std::string ack;
        ack.assign(ackbuf);

        if(mcHubd::UDPServer::isAck(ack))
            ret = true;
    }
    else //Message Receive Timeout or other error
    {
        std::perror("[SEND] Error");
        std::cout << __PRETTY_FUNCTION__ << "[" << __LINE__ << "] " << "recv error" << std::endl;
    }

    std::cout << __PRETTY_FUNCTION__ << "[" << __LINE__ << "] " << "change to default timeout" << std::endl;

    if(mcHubd::UDPServer::rollbackTimeoutOpt(serverfd) == false)
        std::cout << __PRETTY_FUNCTION__ << "[" << __LINE__ << "] " << "Error" << std::endl;

    return ret;
}

std::shared_ptr<mcHubd::SocketData> mcHubd::UDPServer::recv()
{
    int serverfd = this->m_sockfd;
    int recvsize = 0;
    struct sockaddr_in clientaddr;
    unsigned int cliaddrlen = sizeof(clientaddr);
    char buf[MAX_BUF_SIZE] = {0,};
    std::shared_ptr<mcHubd::SocketData> sockData;

    memset(buf, '\0', MAX_BUF_SIZE);
    recvsize = recvfrom(serverfd, (void *)&buf, sizeof(buf), 0,
            (struct sockaddr *)&clientaddr, &cliaddrlen);

    std::cout << __PRETTY_FUNCTION__ << "[" << __LINE__ << "] buf: " << buf << std::endl;
    std::cout << __PRETTY_FUNCTION__ << "[" << __LINE__ << "] recvsize " << recvsize << std::endl;
    if( recvsize > 0)
    {
        std::string msg;
        std::string ack;
        struct timeval tv;
        tv.tv_sec = 1; // 1 second
        tv.tv_usec = 50;

        if(setsockopt(serverfd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof tv) != 0)
        {
            std::perror("Set opt [SO_SNDTIMEO] Error! ");
        }

        //make ack
        msg.assign(buf);
        memset(buf, '\0', MAX_BUF_SIZE);

        ack = mcHubd::UDPServer::makeAck(msg);
        std::cout << __PRETTY_FUNCTION__ << " [" << __LINE__ << "] ack: " << ack << std::endl;

        if(ack.empty() == false)
        {
            //send ack
            int sentsize = 0;
            (char*)memcpy(buf, ack.c_str(), MAX_BUF_SIZE-1);
            sentsize = sendto(serverfd, static_cast<void*>(buf), sizeof(buf), 0,
                    (struct sockaddr*)&clientaddr, cliaddrlen);

            if( sentsize == sizeof(buf) )
            {
                //create sockData
                sockData = std::make_shared<mcHubd::SocketData>(serverfd, clientaddr, msg);
            }
            else
            {
                std::perror("[SEND] Error");
            }
        }
    }

    return sockData;
}

bool mcHubd::UDPServer::isAck(std::string ack)
{
    struct json_object* jobj = NULL;
    struct json_object* typeJobj = NULL;
    std::string type;

    jobj = json_tokener_parse(ack.c_str());

    if((jobj == NULL) || is_error(jobj))
        return false;

    if(!json_object_object_get_ex(jobj, "msgType", &typeJobj))
    {
        json_object_put(jobj);
        return false;
    }

    if(typeJobj)
        type.assign(json_object_get_string(typeJobj));

    if(type.compare("ACK") == 0)
    {
        json_object_put(jobj);
        return true;
    }

    json_object_put(jobj);
    return false;
}

std::string mcHubd::UDPServer::makeAck(std::string msg)
{
    struct json_object* jobj = NULL;
    struct json_object* typeJobj = NULL;
    std::string ackMsg;

    jobj = json_tokener_parse(msg.c_str());

    if((jobj == NULL) || is_error(jobj))
        return ackMsg;

    if(!json_object_object_get_ex(jobj, "msgType", &typeJobj))
    {
        json_object_put(jobj);
        return ackMsg;
    }

    json_object_put(typeJobj);

    typeJobj = json_object_new_string("ACK");
    json_object_object_add(jobj, "msgType", typeJobj);
    ackMsg.assign(json_object_get_string(jobj));

    json_object_put(jobj);
    return ackMsg;
}

bool mcHubd::UDPServer::setTimeoutOpt(int serverfd)
{
    bool ret = true;
    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 50;

    if(setsockopt(serverfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof tv) != 0)
    {
        std::perror("[SO_RCVTIMEO] Error! ");
        ret = false;
    }

    if(setsockopt(serverfd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof tv) != 0)
    {
        std::perror("[SO_REUSEADDR] Error! ");
        ret = false;
    }

    return ret;
}

bool mcHubd::UDPServer::rollbackTimeoutOpt(int serverfd)
{
    bool ret = true;
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;

    if(setsockopt(serverfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof tv) != 0)
    {
        std::perror("[SO_RCVTIMEO] Error! ");
        ret = false;
    }

    if(setsockopt(serverfd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof tv) != 0)
    {
        std::perror("[SO_REUSEADDR] Error! ");
        ret = false;
    }

    return ret;
}

