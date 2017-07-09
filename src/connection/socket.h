#ifndef SOCKET_H_
#define SOCKET_H_

#include "mcHubType.h"
#include "socketData.h"

namespace mcHubd {
    class Socket {
        public:
            Socket(int port);
            virtual ~Socket();
            virtual bool start() = 0;
            virtual int getSockfd();
            virtual std::shared_ptr<SocketData> recv()=0;
            virtual void finish();

        private:
            Socket();

        protected:
            unsigned short int m_port;
            struct sockaddr_in m_serveraddr;
            int m_sockfd;
    };
}

#endif /*SOCKET_H_*/
