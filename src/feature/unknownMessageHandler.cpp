#include "unknownMessageHandler.h"

mcHubd::UnknownMessageHandler::UnknownMessageHandler(){}
mcHubd::UnknownMessageHandler::~UnknownMessageHandler(){}

bool mcHubd::UnknownMessageHandler::request(mcHubd::Message* msg)
{
    std::string emptyMsg;
    mcHubd::RESPCODE code;
    code = MCHUBD_UNDEFINED_TYPE;
    mcHubd::MessageHandler::_responseError(code, emptyMsg, msg);
    return false;
}
