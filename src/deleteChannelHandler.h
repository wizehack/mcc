#ifndef DEL_CHANNEL_HANDLER_H_
#define DEL_CHANNEL_HANDLER_H_

#include "mcHubType.h"
#include "message.h"
#include "messageHandler.h"

namespace mcHubd {
    class DeleteChannelHandler: public MessageHandler{
        public:
            DeleteChannelHandler();
            virtual ~DeleteChannelHandler();
            void request(std::shared_ptr<mcHubd::Message> msg);

        private:
            bool parse(std::string payload);
            static bool _makeResponseMessage(struct json_object** pJobj, std::string cKey, key_t channel);

        private:
            std::string m_cKey;
            key_t m_channel;
    };
}

#endif /* DEL_CHANNEL_HANDLER_H_ */
