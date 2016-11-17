#ifndef MESSAGE_H_
#define MESSAGE_H_

#include "mcHubType.h"

namespace mcHubd {
    class Message {
        public:
            Message(mcHubd::MSGTYPE type);
            ~Message();

            void setData(std::string data);
            void setHeader(std::string header);
            void setBody(std::string body);

            mcHubd::MSGTYPE getType();
            std::string getData();
            std::string getHeader();
            std::string getBody();

        private:
            mcHubd::MSGTYPE m_msgType;
            std::string m_data;
            std::string m_header;
            std::string m_body;
    };
}
#endif /*MESSAGE_H_*/
