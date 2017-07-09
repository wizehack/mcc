#ifndef SOCKET_DATA_H_
#define SOCKET_DATA_H_

#include "mcHubType.h"

namespace mcHubd {
    class SocketData {
        public:
            SocketData(int sockfd, struct sockaddr_in sockaddr, std::string msg);
            ~SocketData();

            int getSockfd() const;
            struct sockaddr_in getSockaddr() const;
            std::string getPayload() const;

        private:
            SocketData();

        private:
            int m_sockfd;
            struct sockaddr_in m_sockaddr;
            std::string m_payload;
    };
}

#endif /*SOCKET_DATA_H_*/
