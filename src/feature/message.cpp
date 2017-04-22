#include "message.h"

mcHubd::Message::Message(mcHubd::MSGTYPE type):
    m_msgType(type),
    m_body(){}
mcHubd::Message::~Message(){}

void mcHubd::Message::setBody(std::string body)
{
    this->m_body = body;
}

mcHubd::MSGTYPE mcHubd::Message::getType() const
{
    return this->m_msgType;
}

std::string mcHubd::Message::getBody() const
{
    return this->m_body;
}
