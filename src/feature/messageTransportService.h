#ifndef MESSAGE_TRANSPORT_SERVICE_H_
#define MESSAGE_TRANSPORT_SERVICE_H_

#include "mcHubType.h"
#include "message.h"

namespace mcHubd {
    class MessageTransportService {
        public:
            MessageTransportService();
            ~MessageTransportService();
            static void sendAll(std::string message);
            static bool sendto(std::string message, mcHubd::Message* msg);

        private:
            static bool udpsend(struct sockaddr_in targetaddr, std::string message);
    };
}

#endif /*MESSAGE_TRANSPORT_SERVICE_H_*/
