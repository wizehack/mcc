#ifndef REG_CHANNEL_HANDLER_H
#define REG_CHANNEL_HANDLER_H

#include "mcHubType.h"
#include "mediator.h"
#include "message.h"
#include "messageHandler.h"

namespace mcHubd {
    class RegisterChannelHandler: public MessageHandler{
        public:
            RegisterChannelHandler();
            virtual ~RegisterChannelHandler();
            bool request(mcHubd::Message* msg);

        private:
            RegisterChannelHandler(const mcHubd::RegisterChannelHandler&);
            RegisterChannelHandler& operator=(const mcHubd::RegisterChannelHandler& rhs);

            bool parse(std::string payload);
            static bool _makeResponseMessage(struct json_object** pJobj, std::string cKey, key_t channel);

        private:
            key_t m_channel;
            std::string m_cKey;
            mcHubd::Message* m_msg;
    };
}


#endif /*REG_CHANNEL_HANDLER_H*/
