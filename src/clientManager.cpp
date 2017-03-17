#include "taskSet.h"
#include "clientManager.h"

mcHubd::ClientManager::ClientManager(mcHubd::Mediator* mediator):
    Manager(mediator, "ClientManager"),
    m_cInfo(mcHubd::ConnectionInfo::getInstance()){}
mcHubd::ClientManager::~ClientManager(){}

void mcHubd::ClientManager::create(mcHubd::Contract** contract)
{
    pid_t pid;

    if((*contract))
    {
        pid = mcHubd::ClientManager::_create((*contract)->getProcessName());
        (*contract)->setProcessId(pid);
    }
}

void mcHubd::ClientManager::add(mcHubd::Contract** contract)
{
    if((*contract))
        this->add((*contract)->getClientKey(), (*contract)->getProcessId());
}

void mcHubd::ClientManager::remove(mcHubd::Contract** contract)
{
    if((*contract))
        this->remove((*contract)->getProcessId());
}

void mcHubd::ClientManager::get(mcHubd::Contract** contract)
{
    if((*contract))
    {
        std::string clientKey((*contract)->getProcessName());
        (*contract)->setProcessId(mcHubd::ClientManager::_get(clientKey));
    }
}

/**
 * add client key and process id
 */
void mcHubd::ClientManager::add(std::string cKey, int pid)
{
    this->m_cInfo->addConnectedClientKeyMap(cKey, static_cast<pid_t>(pid));
}

/**
 * remove connected all of the client key if they have same pid
 */
void mcHubd::ClientManager::remove(int pid)
{
    std::map<std::string, key_t> availableListMap;
    std::map<std::string, key_t>::iterator mItor = availableListMap.begin();

    std::vector<std::string> psVector;
    std::vector<std::string>::iterator vItor;

    availableListMap = this->m_cInfo->getAvailableList();
    psVector = this->m_cInfo->getConnectedClientKey(static_cast<pid_t>(pid));

    for(vItor = psVector.begin(); vItor != psVector.end(); ++vItor)
    {
        mcHubd::Contract* contract = new mcHubd::Contract();
        contract->setClientKey((*vItor));

        mItor = availableListMap.find((*vItor));

        if(mItor != availableListMap.end())
            contract->setChannel(mItor->second);

        this->m_mediator->deleteChannel(&contract);
        delete contract;
    }

    this->m_cInfo->deleteConnectedProcess(pid);
}

/**
 * get pid
 */
pid_t mcHubd::ClientManager::_get(std::string& cKey)
{
    return -1;
}

/**
 * launch process
 */
pid_t mcHubd::ClientManager::_create(std::string processName)
{
    pid_t pid = 0;
    return static_cast<pid_t>(pid);
}
