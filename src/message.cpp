#include "message.h"

mcHubd::Message::Message(mcHubd::MSGTYPE type):
    m_msgType(type),
    m_data(),
    m_header(),
    m_body(){}
mcHubd::Message::~Message(){}

void mcHubd::Message::setData(std::string data)
{
    this->m_data = data;
}

void mcHubd::Message::setHeader(std::string header)
{
    this->m_header = header;
}

void mcHubd::Message::setBody(std::string body)
{
    this->m_body = body;
}

mcHubd::MSGTYPE mcHubd::Message::getType()
{
    return this->m_msgType;
}

std::string mcHubd::Message::getData()
{
    return this->m_data;
}

std::string mcHubd::Message::getHeader()
{
    return this->m_header;
}

std::string mcHubd::Message::getBody()
{
    return this->m_body;
}
