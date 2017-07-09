#ifndef MESSAGE_H_
#define MESSAGE_H_

#include "mcHubType.h"

namespace mcHubd {
    class Message {
        public:
            Message(mcHubd::MSGTYPE type);
            ~Message();

            void setBody(std::string body);
            void setConn(struct sockaddr_in sockAddr);

            mcHubd::MSGTYPE getType() const;
            std::string getBody() const;
            struct sockaddr_in getSockAddr() const;

        private:
            mcHubd::MSGTYPE m_msgType;
            std::string m_body;
            struct sockaddr_in m_sockAddr;


    };
}
#endif /*MESSAGE_H_*/
