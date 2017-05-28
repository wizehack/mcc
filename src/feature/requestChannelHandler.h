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
            RequestChannelHandler(const mcHubd::RequestChannelHandler&);
            RequestChannelHandler& operator=(const mcHubd::RequestChannelHandler& rhs);

            bool parse(std::string payload);
            static bool _makeResponseMessage(struct json_object** pJobj, std::string cKey, key_t channel, std::string status);

        private:
            std::string m_cKey;
            mcHubd::Message* m_msg;
    };
}

#endif /*REQ_CHANNEL_HANDLER_H_*/
