#ifndef UDP_CLIENT_h_
#define UDP_CLIENT_h_

#include "socketClient.h"

class UDPClient : public SocketClient {
    public:
        UDPClient(std::string ipAddr, int port, const int MAXBUF);
        ~UDPClient();
        bool conn();
        std::string send(std::string data);
        std::string receive();
        std::string getSubscribe();

    private:
        UDPClient();
        ssize_t wrap_recvfrom(int sockfd, void *buf, size_t len, int flags,
                                 struct sockaddr *src_addr, socklen_t *addrlen);

        static std::string makeAck(std::string& msg);
        static bool isSubscription(std::string& msg);

    private:
        struct sockaddr_in m_servaddr;
};

#endif /*UDP_CLIENT_h_*/
