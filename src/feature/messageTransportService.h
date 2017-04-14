#ifndef MESSAGE_TRANSPORT_SERVICE_H_
#define MESSAGE_TRANSPORT_SERVICE_H_

#include "mcHubType.h"

namespace mcHubd {
    class MessageTransportService {
        public:
            MessageTransportService();
            ~MessageTransportService();
            void sendAll(std::string message);
    };
}

#endif /*MESSAGE_TRANSPORT_SERVICE_H_*/
