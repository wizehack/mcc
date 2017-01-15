#include "channelManager.h"
#include "taskSet.h"
#include "clientManager.h"

mcHubd::ChannelManager::ChannelManager(mcHubd::Mediator* mediator):
    Manager(mediator, "ChannelManager"),
    m_tSet(mcHubd::TaskSet::getInstance()),
    MAX(10){}
mcHubd::ChannelManager::~ChannelManager(){}

void mcHubd::ChannelManager::create(mcHubd::Contract** contract)
{
    key_t ch = -1;

    if((*contract))
    {
        ch = this->createNewChannel((*contract)->getClientKey(), (*contract)->getContractID());
        this->m_mediator->notify((*contract), NOTI_INIT);
    }

    (*contract)->setChannel(ch);
}

void mcHubd::ChannelManager::add(mcHubd::Contract** contract)
{
    std::string clientKey((*contract)->getClientKey());
    key_t channel = (*contract)->getChannel();

    if(this->isAvailable(clientKey))
    {
        (*contract)->setRespCode(MCHUBD_IS_AVAILABLE_KEY);
        return;
    }

    if(mcHubd::TaskSet::getInstance()->isWaitingTask(clientKey))
    {
        mcHubd::ConnectionInfo::getInstance()->addAvailableKey(clientKey, channel);
        mcHubd::TaskSet::getInstance()->rmWaitToReady(clientKey);
        this->m_mediator->notify((*contract), NOTI_CHANNEL_READY);
    }
    else
    {
        (*contract)->setRespCode(MCHUBD_NOT_ACCEPTABLE_KEY);
    }
}

void mcHubd::ChannelManager::remove(mcHubd::Contract** contract)
{
    std::map<std::string, key_t> chMap;
    std::map<std::string, key_t>::iterator it;
    std::string cKey;
    cKey = (*contract)->getClientKey();

    if(this->m_tSet->isWaitingTask(cKey))
    {
        this->m_tSet->rmWaitToReady(cKey);
        (*contract)->setRespCode(MCHUBD_OK);
        return;
    }

    chMap = this->m_cInfo->getAvailableList();

    if(chMap.empty())
    {
        (*contract)->setRespCode(MCHUBD_INTERNAL_ERROR);
        return;
    }

    it = chMap.find(cKey);

    if(it != chMap.end())
    {
        key_t ch = it->second;

        if(ch != (*contract)->getChannel())
        {
            (*contract)->setRespCode(MCHUBD_INFORM_CHANNEL_ERROR);
            return;
        }

        this->m_cInfo->removeAvailalbeKey(cKey);
        mcHubd::ChannelManager::_removeChannel(ch);
        (*contract)->setRespCode(MCHUBD_OK);
    }
    else
    {
        (*contract)->setRespCode(MCHUBD_INTERNAL_ERROR);
    }
}

void mcHubd::ChannelManager::get(mcHubd::Contract** contract)
{
}

key_t mcHubd::ChannelManager::createNewChannel(std::string cKey, int id)
{
    int shmid = -1;
    int tryCount = 0;
    key_t ch = -1;

    if(this->isAccepted(cKey) == false)
        return ch;

    if(this->m_tSet->isWaitingTask(cKey))
        return ch;

    if(this->isAvailable(cKey))
        return ch;

    ch = this->makeChannelNumber(cKey, id);

    if(ch < 0)
    {
        return ch;
    }

    if(ch == 0)
    {
        return -1;
    }

    shmid = shmget(ch, 1024, IPC_CREAT | IPC_EXCL| 0666);

    while((shmid == -1) && (tryCount < MAX))
    {
        key_t originCh = ch;

        tryCount++;

        ch = ch * 10 + static_cast<key_t>(tryCount);

        shmid = shmget(ch, 1024, IPC_CREAT | IPC_EXCL| 0666);

        if(shmid > 0)
            break;

        ch = originCh;
    }

    if(shmid == -1)
    {
        perror("shmget failed");
        return -1;
    }

    if(shmctl(shmid, IPC_RMID, 0) == -1)
    {
        perror("shnctl failed");
    }

    this->m_tSet->addWaitToReadyTask(cKey);
    return ch;
}

key_t mcHubd::ChannelManager::makeChannelNumber(std::string& cKey, int id)
{
    std::map<std::string, pid_t> conMap;
    std::map<std::string, pid_t>::iterator it;
    key_t ch = -1;

    if(id >= MAX)
        return ch;

    conMap = this->m_cInfo->getConnectedClientKeyMap();
    it = conMap.find(cKey);

    if(it != conMap.end())
    {
        pid_t pid = it->second;
        ch = static_cast<key_t>(pid * 10 + id); //create unique channel number
    }

    return ch;
}

void mcHubd::ChannelManager::_removeChannel(key_t ch)
{
    if(ch > 0)
    {
        int shmid = 0;

        if((shmid = shmget(ch, 1024, IPC_CREAT| 0666)) == -1)
        {
            perror("shmget error");
        }

        if(shmctl(shmid, IPC_RMID, 0) == -1)
        {
            perror("shnctl failed");
        }
    }
}
