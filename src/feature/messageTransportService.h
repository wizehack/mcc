#ifndef MESSAGE_TRANSPORT_SERVICE_H_
#define MESSAGE_TRANSPORT_SERVICE_H_

#include "mcHubType.h"
#include "message.h"

namespace mcHubd {
    class MessageTransportService {
        public:
            MessageTransportService();
            ~MessageTransportService();
            void sendAll(std::string message);
            void sendto(std::string message, mcHubd::Message* msg);
    };
}

#endif /*MESSAGE_TRANSPORT_SERVICE_H_*/
