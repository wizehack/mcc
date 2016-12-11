#include "channelStatusMediator.h"
#include "messageHandler.h"
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

void mcHubd::ChannelStatusMediator::removeManager(std::shared_ptr<mcHubd::Manager> manager)
{
    this->m_mgrList.remove(manager);
}

void mcHubd::ChannelStatusMediator::getNewChannel(mcHubd::Contract** pContract)
{
    mcHubd::RESPCODE code;
    std::string cKey;

    if((*pContract) == NULL)
        return;

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

void mcHubd::ChannelStatusMediator::registerNewChannel(mcHubd::Contract** pContract)
{
    mcHubd::Manager* channelMgr = NULL;
    std::string role("ChannelManager");

    channelMgr = this->getManager(role);
    channelMgr->add(pContract);
}

void mcHubd::ChannelStatusMediator::deleteClient(mcHubd::Contract** pContract)
{
    mcHubd::Manager* clientMgr = NULL;
    std::string roleClientMgr("ClientManager");

    if((*pContract) == NULL)
    {
        (*pContract)->setRespCode(MCHUBD_INTERNAL_ERROR);
        return;
    }

    clientMgr = this->getManager(roleClientMgr);

    if(clientMgr == NULL)
    {
        (*pContract)->setRespCode(MCHUBD_INTERNAL_ERROR);
        return;
    }

    clientMgr->remove(pContract);
    (*pContract)->setRespCode(MCHUBD_OK);
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
    mcHubd::Manager* channelMgr = NULL;
    mcHubd::Manager* clientMgr = NULL;
    std::string roleChannelMgr("ChannelManager");
    std::string roleClientMgr("ClientManager");

    if((*pContract) == NULL)
        return false;

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

void mcHubd::ChannelStatusMediator::notifyCreateChannel(std::string cKey, key_t channel)
{
}

mcHubd::Manager* mcHubd::ChannelStatusMediator::getManager(std::string& role)
{
    std::list<std::shared_ptr<mcHubd::Manager>>::iterator itor;

    for(itor = this->m_mgrList.begin(); itor != this->m_mgrList.end(); itor++)
    {
        if((*itor)->getRole().compare(role) == 0)
            return (*itor).get();
    }

    return NULL;
}
