#ifndef REQ_UNKNOWN_MESSAGE_HANDLER_H_
#define REQ_UNKNOWN_MESSAGE_HANDLER_H_

#include "mcHubType.h"
#include "messageHandler.h"
#include "message.h"

namespace mcHubd {
    class UnknownMessageHandler: public MessageHandler{
        public:
            UnknownMessageHandler();
            virtual ~UnknownMessageHandler();
            bool request(mcHubd::Message* msg);

        private:
            UnknownMessageHandler(const mcHubd::UnknownMessageHandler&);
            UnknownMessageHandler& operator=(const mcHubd::UnknownMessageHandler& rhs);
    };
}

#endif /*REQ_UNKNOWN_MESSAGE_HANDLER_H_*/
