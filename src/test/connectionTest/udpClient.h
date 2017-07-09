#ifndef UDP_CLIENT_h_
#define UDP_CLIENT_h_

#include "socketClient.h"

class UDPClient : public SocketClient {
    public:
        UDPClient(std::string ipAddr, int port, const int MAXBUF);
        ~UDPClient();
        bool conn();
        std::string send(std::string data);

    private:
        UDPClient();
        static std::string makeAck(std::string msg);

    private:
        struct sockaddr_in m_servaddr;
};

#endif /*UDP_CLIENT_h_*/
