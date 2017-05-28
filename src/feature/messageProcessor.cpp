#include "messageProcessor.h"
#include "messageHandler.h"
#include "registerChannelHandler.h"
#include "registerClientHandler.h"
#include "deleteChannelHandler.h"
#include "deleteClientHandler.h"
#include "requestChannelHandler.h"

mcHubd::MessageProcessor::MessageProcessor(){}
mcHubd::MessageProcessor::~MessageProcessor(){}

bool mcHubd::MessageProcessor::request(mcHubd::Message* msg)
{
    if(msg)
    {
        bool bResult = false;
        mcHubd::MessageHandler* regClientHandler = new mcHubd::RegisterClientHandler();
        mcHubd::MessageHandler* regChannelHandler = new mcHubd::RegisterChannelHandler();
        mcHubd::MessageHandler* delClientHandler = new mcHubd::DeleteClientHandler();
        mcHubd::MessageHandler* delChannelHandler = new mcHubd::DeleteChannelHandler();
        mcHubd::MessageHandler* requestChannelHandler = new mcHubd::RequestChannelHandler();

        regClientHandler->setNext(regChannelHandler);
        regChannelHandler->setNext(delClientHandler);
        delClientHandler->setNext(delChannelHandler);
        delChannelHandler->setNext(requestChannelHandler);

        bResult = regClientHandler->request(msg);

        delete regClientHandler;
        delete regChannelHandler;
        delete delClientHandler;
        delete delChannelHandler;
        delete requestChannelHandler;

        return bResult;
    }

    return false;
}
