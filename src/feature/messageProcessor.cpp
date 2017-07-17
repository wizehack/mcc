#include "messageProcessor.h"
#include "messageHandler.h"
#include "registerChannelHandler.h"
#include "registerClientHandler.h"
#include "deleteChannelHandler.h"
#include "deleteClientHandler.h"
#include "requestChannelHandler.h"
#include "unknownMessageHandler.h"

mcHubd::MessageProcessor::MessageProcessor(){}
mcHubd::MessageProcessor::~MessageProcessor(){}

bool mcHubd::MessageProcessor::_request(mcHubd::Message* msg)
{
    if(msg)
    {
        bool bResult;
        mcHubd::MessageHandler* regClientHandler = new mcHubd::RegisterClientHandler();
        mcHubd::MessageHandler* regChannelHandler = new mcHubd::RegisterChannelHandler();
        mcHubd::MessageHandler* delClientHandler = new mcHubd::DeleteClientHandler();
        mcHubd::MessageHandler* delChannelHandler = new mcHubd::DeleteChannelHandler();
        mcHubd::MessageHandler* requestChannelHandler = new mcHubd::RequestChannelHandler();
        mcHubd::MessageHandler* unknownMessageHandler = new mcHubd::UnknownMessageHandler();

        regClientHandler->setNext(regChannelHandler);
        regChannelHandler->setNext(delClientHandler);
        delClientHandler->setNext(delChannelHandler);
        delChannelHandler->setNext(requestChannelHandler);
        requestChannelHandler->setNext(unknownMessageHandler);

        bResult = regClientHandler->request(msg);

        if(regClientHandler)
            delete regClientHandler;

        if(regChannelHandler)
            delete regChannelHandler;

        if(delClientHandler)
            delete delClientHandler;

        if(delChannelHandler)
            delete delChannelHandler;

        if(requestChannelHandler)
            delete requestChannelHandler;

        if(unknownMessageHandler)
            delete unknownMessageHandler;

        return bResult;
    }

    return false;
}
