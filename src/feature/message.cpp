#include "message.h"

mcHubd::Message::Message(mcHubd::MSGTYPE type):
    m_msgType(type),
    m_body(),
    m_sockfd(0),
    m_sockAddr(){}
mcHubd::Message::~Message(){}

void mcHubd::Message::setBody(std::string body)
{
    this->m_body = body;
}

void mcHubd::Message::setConn(struct sockaddr_in sockAddr, int sockfd)
{
    this->m_sockAddr = sockAddr;
    this->m_sockfd = sockfd;
}

mcHubd::MSGTYPE mcHubd::Message::getType() const
{
    return this->m_msgType;
}

std::string mcHubd::Message::getBody() const
{
    return this->m_body;
}

struct sockaddr_in mcHubd::Message::getSockAddr() const
{
    return this->m_sockAddr;
}

int mcHubd::Message::getSockfd() const
{
    return this->m_sockfd;
}
