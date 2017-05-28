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
            bool request(mcHubd::Message* msg);

        private:
            DeleteChannelHandler(const mcHubd::DeleteChannelHandler&);
            DeleteChannelHandler& operator=(const mcHubd::DeleteChannelHandler& rhs);

            bool parse(std::string payload);
            static bool _makeResponseMessage(struct json_object** pJobj, std::string cKey, key_t channel);

        private:
            std::string m_cKey;
            key_t m_channel;
            mcHubd::Message* m_msg;
    };
}

#endif /* DEL_CHANNEL_HANDLER_H_ */
