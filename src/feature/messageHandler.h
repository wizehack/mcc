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
            virtual bool request(mcHubd::Message* msg) = 0;

        protected:
            static void _responseError(mcHubd::RESPCODE code, std::string extraMsg, mcHubd::Message* msg);
            static void _responseOK(std::string respMsg, mcHubd::Message* msg);

        private:
            MessageHandler(const mcHubd::MessageHandler&);
            MessageHandler& operator=(const mcHubd::MessageHandler& rhs);
            static std::string _getErrorMessage(mcHubd::RESPCODE code);

        protected:
            mcHubd::MessageHandler* m_next;
    };
}

#endif /* MESSAGE_HANDLER_H_ */
