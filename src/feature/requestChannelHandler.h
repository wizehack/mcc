#ifndef REQ_CHANNEL_HANDLER_H_
#define REQ_CHANNEL_HANDLER_H_

#include "mcHubType.h"
#include "message.h"
#include "messageHandler.h"

namespace mcHubd {
    class RequestChannelHandler: public MessageHandler{
        public:
            RequestChannelHandler();
            virtual ~RequestChannelHandler();
            bool request(mcHubd::Message* msg);

        private:
            bool parse(std::string payload);
            static bool _makeResponseMessage(struct json_object** pJobj, std::string cKey, key_t channel, std::string status);

        private:
            std::string m_cKey;
    };
}

#endif /*REQ_CHANNEL_HANDLER_H_*/
