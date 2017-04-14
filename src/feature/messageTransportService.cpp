#include "messageTransportService.h"
#include "testStub.h"

mcHubd::MessageTransportService::MessageTransportService(){}
mcHubd::MessageTransportService::~MessageTransportService(){}
void mcHubd::MessageTransportService::sendAll(std::string message)
{
    mcHubd::TestStub::getInstance()->addSubscribeMsg(message); //Test Code
}
