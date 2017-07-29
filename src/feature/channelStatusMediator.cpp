#include "channelStatusMediator.h"
#include "messageHandler.h"
#include "messageTransportService.h"
#include "taskSet.h"

mcHubd::ChannelStatusMediator::ChannelStatusMediator():
    m_mgrList(){}
mcHubd::ChannelStatusMediator::~ChannelStatusMediator()
{
    this->m_mgrList.clear();
}

void mcHubd::ChannelStatusMediator::appendManager(std::shared_ptr<mcHubd::Manager> manager)
{
    this->m_mgrList.push_back(manager);
}

void mcHubd::ChannelStatusMediator::getNewChannel(mcHubd::Contract** pContract)
{
    mcHubd::RESPCODE code;
    std::string cKey;

    if((*pContract))
    {
        cKey = (*pContract)->getClientKey();
        code = this->checkRegisterStatus(cKey);

        if(code != MCHUBD_OK)
        {
            (*pContract)->setRespCode(code);
            return;
        }

        if(this->createNewChannel(pContract) == false)
        {
            (*pContract)->setRespCode(MCHUBD_CREATE_CHANNEL_ERROR);
        }

        (*pContract)->setRespCode(MCHUBD_OK);
    }
}

void mcHubd::ChannelStatusMediator::getChannel(mcHubd::Contract** pContract)
{
    if((*pContract))
    {
        std::string role("ChannelManager");
        mcHubd::Manager* channelMgr = this->getManager(role);
        channelMgr->get(pContract);
    }
}

void mcHubd::ChannelStatusMediator::registerNewChannel(mcHubd::Contract** pContract)
{
    mcHubd::Manager* channelMgr = NULL;
    std::string role("ChannelManager");

    channelMgr = this->getManager(role);
    channelMgr->add(pContract);
}

void mcHubd::ChannelStatusMediator::deleteClient(mcHubd::Contract** pContract)
{
    std::string roleClientMgr("ClientManager");

    if((*pContract))
    {
        mcHubd::Manager* clientMgr = NULL;
        clientMgr = this->getManager(roleClientMgr);

        if(clientMgr == NULL)
        {
            (*pContract)->setRespCode(MCHUBD_INTERNAL_ERROR);
            return;
        }

        clientMgr->remove(pContract);
    }
}

void mcHubd::ChannelStatusMediator::deleteChannel(mcHubd::Contract** pContract)
{
    mcHubd::Manager* channelMgr = NULL;
    std::string role("ChannelManager");

    channelMgr = this->getManager(role);
    channelMgr->remove(pContract);
}

void mcHubd::ChannelStatusMediator::notify(mcHubd::Contract* contract, mcHubd::CONTRACTREASON reason)
{
    std::string msg;
    mcHubd::MessageTransportService mts;

    if(reason == NOTI_CHANNEL_OPEN)
    {
        contract->setChannelStatus(OPEN);
    }
    else if(reason == NOTI_CHANNEL_READY)
    {
        contract->setChannelStatus(READY);
    }
    else if(reason == NOTI_CHANNEL_CLOSE)
    {
        contract->setChannelStatus(CLOSE);
    }

    msg = mcHubd::ChannelStatusMediator::getChannelStatusMessage(contract, true);
    mts.sendAll(msg);
}

std::string mcHubd::ChannelStatusMediator::getChannelStatusMessage(mcHubd::Contract* contract, bool bSubscribe)
{
    std::string msg;
    std::string state;

    state = mcHubd::ChannelStatusMediator::getChannelStatus(contract);

    if(state.empty() == false)
    {
        std::string key = contract->getClientKey();
        int channel = static_cast<int>(contract->getChannel());

        if( key.empty() == false )
        {
            struct json_object* jobj = NULL;
            struct json_object* codeJobj = NULL;
            struct json_object* retJobj = NULL;
            struct json_object* messageJobj = NULL;
            struct json_object* keyJobj = NULL;
            struct json_object* channelJobj = NULL;
            struct json_object* stateJobj = NULL;

            jobj = json_object_new_object();
            codeJobj = json_object_new_int(MCHUBD_OK);
            retJobj = json_object_new_boolean(true);

            messageJobj = json_object_new_object();
            keyJobj = json_object_new_string(key.c_str());
            channelJobj = json_object_new_int(channel);
            stateJobj = json_object_new_string(state.c_str());

            json_object_object_add(messageJobj, "key", keyJobj);
            json_object_object_add(messageJobj, "channel", channelJobj);
            json_object_object_add(messageJobj, "state", stateJobj);

            if(bSubscribe)
                json_object_object_add(jobj, "msgType", json_object_new_string("subscribe"));
            else
                json_object_object_add(jobj, "msgType", json_object_new_string("response"));

            json_object_object_add(jobj, "code", codeJobj);
            json_object_object_add(jobj, "message", messageJobj);
            json_object_object_add(jobj, "return", retJobj);

            msg.assign(json_object_get_string(jobj));
            json_object_put(jobj);
        }
    }

    return msg;
}

std::string mcHubd::ChannelStatusMediator::getChannelStatus(mcHubd::Contract* contract)
{
    std::string state;

    if(contract->getChannelStatus() == OPEN)
    {
        state.assign("open");
    }
    else if(contract->getChannelStatus() == READY)
    {
        state.assign("ready");
    }
    else if(contract->getChannelStatus() == CLOSE)
    {
        state.assign("closed");
    }

    return state;
}

mcHubd::RESPCODE mcHubd::ChannelStatusMediator::checkRegisterStatus(std::string& cKey)
{
    mcHubd::Manager* clientMgr = NULL;
    std::string role("ClientManager");

    clientMgr = this->getManager(role);

    if(clientMgr == NULL)
    {
        return MCHUBD_INTERNAL_ERROR;
    }

    if(clientMgr->isAccepted(cKey) == false)
    {
        return MCHUBD_NOT_ACCEPTABLE_KEY;
    }

    if(mcHubd::TaskSet::getInstance()->isWaitingTask(cKey))
    {
        return MCHUBD_IS_CREATED_KEY;
    }

    if(clientMgr->isAvailable(cKey))
    {
        return MCHUBD_IS_AVAILABLE_KEY;
    }

    return MCHUBD_OK;
}

bool mcHubd::ChannelStatusMediator::createNewChannel(mcHubd::Contract** pContract)
{
    std::string roleChannelMgr("ChannelManager");
    std::string roleClientMgr("ClientManager");

    if((*pContract))
    {
        mcHubd::Manager* channelMgr = NULL;
        mcHubd::Manager* clientMgr = NULL;

        channelMgr = this->getManager(roleChannelMgr);
        clientMgr = this->getManager(roleClientMgr);

        if((clientMgr == NULL) || (channelMgr == NULL))
        {
            return false;
        }

        if( ((*pContract)->getProcessId() > 0) &&
                ((*pContract)->getClientKey().empty() == false) &&
                ((*pContract)->getProcessName().empty() == false) )
        {
            clientMgr->add(pContract); // register process id
            channelMgr->create(pContract); //new channel is created
        }

        if((*pContract)->getChannel() < 0)
            return false;

        return true;
    }

    return false;
}

mcHubd::Manager* mcHubd::ChannelStatusMediator::getManager(std::string& role)
{
    std::list<std::shared_ptr<mcHubd::Manager>>::iterator itor;

    for(itor = this->m_mgrList.begin(); itor != this->m_mgrList.end(); ++itor)
    {
        if((*itor)->getRole().compare(role) == 0)
            return (*itor).get();
    }

    return NULL;
}
