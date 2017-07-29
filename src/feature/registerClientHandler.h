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
            bool request(mcHubd::Message* msg);

        private:
            RegisterClientHandler(const mcHubd::RegisterClientHandler&);
            RegisterClientHandler& operator=(const mcHubd::RegisterClientHandler& rhs);

            bool parse(std::string payload);
            std::map<std::string, key_t> makeNewChannelList(mcHubd::Mediator* mediator);
            static void _responseOK(std::map<std::string, key_t> keyChannelMap, mcHubd::Message* msg);

        private:
            pid_t m_pid;
            std::string m_processName;
            bool m_subscribe;
            const unsigned int MAX_NUMBER_OF_CLIENT_KEY;
            std::list<std::string> m_cKeyList;
            mcHubd::Message* m_msg;
    };
}

#endif /* REQ_CLIENT_HANDLER_H_ */
