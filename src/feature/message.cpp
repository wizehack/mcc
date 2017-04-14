#include "message.h"

mcHubd::Message::Message(mcHubd::MSGTYPE type):
    m_msgType(type),
    m_header(),
    m_body(){}
mcHubd::Message::~Message(){}

void mcHubd::Message::setHeader(std::string header)
{
    this->m_header = header;
}

void mcHubd::Message::setBody(std::string body)
{
    this->m_body = body;
}

mcHubd::MSGTYPE mcHubd::Message::getType() const
{
    return this->m_msgType;
}

std::string mcHubd::Message::getHeader() const
{
    return this->m_header;
}

std::string mcHubd::Message::getBody() const
{
    return this->m_body;
}
