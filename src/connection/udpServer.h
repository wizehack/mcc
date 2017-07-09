#ifndef UDP_SOCKET_H_
#define UDP_SOCKET_H_

#include "socket.h"

#define MAX_BUF_SIZE 1024
namespace mcHubd {
    class UDPServer : public Socket {
        public:
            UDPServer(int port);
            ~UDPServer();
            bool start();
            struct sockaddr_in getSockAddr() const;
            std::shared_ptr<SocketData> recv();
            static bool sendmsg(int serverfd, struct sockaddr_in targetaddr, std::string msg);

        protected:
            static bool setTimeoutOpt(int serverfd);
            static bool rollbackTimeoutOpt(int serverfd);

        private:
            static std::string makeAck(std::string msg);
            static bool isAck(std::string ack);

        private:
            UDPServer();

    };
}
#endif /*UDP_SOCKET_H_*/
