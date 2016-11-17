#ifndef MESSAGE_HANDLER_H_
#define MESSAGE_HANDLER_H_

#include "mcHubType.h"
#include "message.h"

namespace mcHubd {
    class MessageHandler{
        public:
            MessageHandler();
            virtual ~MessageHandler();

            void setNext(mcHubd::MessageHandler* handler);
            virtual void request(std::shared_ptr<mcHubd::Message> msg) = 0;

        protected:
            void responseError(mcHubd::RESPCODE code, std::string extraMsg);
            void responseOK(std::string respMsg);

        private:
            std::string getMessage(mcHubd::RESPCODE code);

        protected:
            mcHubd::MessageHandler* m_next;
    };
}

#endif /* MESSAGE_HANDLER_H_ */
