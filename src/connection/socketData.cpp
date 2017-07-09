#include "socketData.h"

mcHubd::SocketData::SocketData(int sockfd, struct sockaddr_in sockaddr, std::string msg):
    m_sockfd(sockfd),
    m_sockaddr(sockaddr),
    m_payload(msg){}

mcHubd::SocketData::~SocketData(){}

int mcHubd::SocketData::getSockfd() const
{
    return this->m_sockfd;
}

struct sockaddr_in mcHubd::SocketData::getSockaddr() const
{
    return this->m_sockaddr;
}

std::string mcHubd::SocketData::getPayload() const
{
    return m_payload;
}
