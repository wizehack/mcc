#ifndef SOCKET_CLIENT_H_
#define SOCKET_CLIENT_H_

#include <string>
#include <iostream>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

class SocketClient {
    public:
        SocketClient(std::string ipAddr, int port, const int MAXBUF);
        virtual ~SocketClient();
        virtual bool conn() = 0;
        virtual std::string send(std::string data) = 0;
        void quit();

    private:
        SocketClient();

    protected:
        std::string m_ipAddr;
        int m_port;
        const int MAX_SIZE;
        int m_socket;
};

#endif /*SOCKET_CLIENT_H_*/
