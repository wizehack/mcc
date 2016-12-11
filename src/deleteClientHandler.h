#ifndef DEL_CLIENT_HANDLER_H_
#define DEL_CLIENT_HANDLER_H_

#include "mcHubType.h"
#include "message.h"
#include "messageHandler.h"

namespace mcHubd {
    class DeleteClientHandler: public MessageHandler{
        public:
            DeleteClientHandler();
            virtual ~DeleteClientHandler();
            void request(std::shared_ptr<mcHubd::Message> msg);

        private:
            bool parse(std::string payload);
            bool makeResponseMessage(struct json_object** pJobj, std::string psName, pid_t pid);

        private:
            pid_t m_pid;
            std::string m_processName;
    };
}

#endif /* DEL_CLIENT_HANDLER_H_ */
