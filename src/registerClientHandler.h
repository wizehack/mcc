#ifndef REQ_CLIENT_HANDLER_H_
#define REQ_CLIENT_HANDLER_H_

#include "mcHubType.h"
#include "mediator.h"
#include "message.h"
#include "messageHandler.h"

namespace mcHubd {
    class RegisterClientHandler: public MessageHandler{
        public:
            RegisterClientHandler();
            virtual ~RegisterClientHandler();
            void request(mcHubd::Message* msg);

        private:
            bool parse(std::string payload);
            std::string makeNewChannelList(mcHubd::Mediator* mediator);

            static bool _createChannelJobj(struct json_object** jobj, std::string cKey, key_t channel);

        private:
            pid_t m_pid;
            std::string m_processName;
            const unsigned int MAX_NUMBER_OF_CLIENT_KEY;
            std::list<std::string> m_cKeyList;
    };
}

#endif /* REQ_CLIENT_HANDLER_H_ */
