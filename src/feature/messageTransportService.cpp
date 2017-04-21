#include "messageTransportService.h"
#include "testStub.h"

mcHubd::MessageTransportService::MessageTransportService(){}
mcHubd::MessageTransportService::~MessageTransportService(){}
void mcHubd::MessageTransportService::sendAll(std::string message)
{
#if FEAT_TEST_MODE
    mcHubd::TestStub::getInstance()->addSubscribeMsg(message); //Test Code
#endif
}
