#include "socket.h"

mcHubd::Socket::Socket(int port) :
    m_port(port),
    m_serveraddr(),
    m_sockfd(-1){}

mcHubd::Socket::~Socket()
{
    if(this->m_sockfd != -1)
        close(this->m_sockfd);
}

int mcHubd::Socket::getSockfd()
{
    return this->m_sockfd;
}

void mcHubd::Socket::finish()
{
    if(this->m_sockfd != -1)
    {
        close(this->m_sockfd);
        this->m_sockfd = -1;
    }
}
