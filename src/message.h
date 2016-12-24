#ifndef MESSAGE_H_
#define MESSAGE_H_

#include "mcHubType.h"

namespace mcHubd {
    class Message {
        public:
            Message(mcHubd::MSGTYPE type);
            ~Message();

            void setHeader(std::string header);
            void setBody(std::string body);

            mcHubd::MSGTYPE getType() const;
            std::string getHeader() const;
            std::string getBody() const;

        private:
            mcHubd::MSGTYPE m_msgType;
            std::string m_header;
            std::string m_body;
    };
}
#endif /*MESSAGE_H_*/
