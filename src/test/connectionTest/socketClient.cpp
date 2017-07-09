#include "socketClient.h"

SocketClient::SocketClient(std::string ipAddr, int port, const int MAXBUF):
    m_ipAddr(ipAddr),
    m_port(port),
    MAX_SIZE(MAXBUF),
    m_socket(-1){}
SocketClient::~SocketClient()
{
    if(this->m_socket > -1)
        close(this->m_socket);
}

void SocketClient::quit()
{
    if(this->m_socket > -1)
    {
        close(this->m_socket);
        this->m_socket = -1;
    }
}
