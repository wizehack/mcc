#ifndef CONMAN_H_
#define CONMAN_H_

#include <stdlib.h>
#include "mcHubType.h"
#include "udpServer.h"

namespace mcHubd {
    class Message;
    class ConnectionManager {
        public:
            ~ConnectionManager();
            static ConnectionManager* getInstance();
            static void start();
            UDPServer* getUDPServer();

            static mcHubd::Message* _getMessageObject(std::shared_ptr<mcHubd::SocketData> sockData);
            static bool _processMessage(mcHubd::Message* msg);
            static void _listen();

        private:
            ConnectionManager();
            ConnectionManager(const ConnectionManager&);
            void operator=(const ConnectionManager&);

        private:
            UDPServer* m_udpServer;
            int m_port;

        private:
            static std::atomic<ConnectionManager*> _singleton;
            static std::mutex _mutex;
    };
}

#endif /*CONMAN_H_*/
