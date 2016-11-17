#include "connectionInfo.h"

std::atomic<mcHubd::ConnectionInfo*> mcHubd::ConnectionInfo::_singleton;
std::mutex mcHubd::ConnectionInfo::_mutex;

mcHubd::ConnectionInfo::ConnectionInfo(){}
mcHubd::ConnectionInfo::~ConnectionInfo(){}

mcHubd::ConnectionInfo* mcHubd::ConnectionInfo::getInstance() {
    mcHubd::ConnectionInfo* inst = _singleton.load(std::memory_order_relaxed);
    std::atomic_thread_fence(std::memory_order_acquire);
    if(inst == 0) {
        std::lock_guard<std::mutex> lock(_mutex);
        inst = _singleton.load(std::memory_order_relaxed);
        if(inst == 0) {
            inst = new mcHubd::ConnectionInfo();
            std::atomic_thread_fence(std::memory_order_release);
            _singleton.store(inst, std::memory_order_relaxed);
        }
    }
    return inst;
}

void mcHubd::ConnectionInfo::addAcceptedKey(std::string key)
{
    std::lock_guard<std::mutex> lock(_mutex);
    this->m_acceptedList.push_back(key);
}

void mcHubd::ConnectionInfo::addConnectedClientKeyMap(std::string cKey, pid_t pid)
{
    std::lock_guard<std::mutex> lock(_mutex);
    this->m_connectedProcessMap.insert(std::pair<std::string, pid_t>(cKey, pid));
}

void mcHubd::ConnectionInfo::addAvailableKey(std::string key, key_t ch)
{
    std::lock_guard<std::mutex> lock(_mutex);
    this->m_availableListMap.insert(std::pair<std::string, key_t>(key, ch));
}

void mcHubd::ConnectionInfo::clearAcceptedKeyList()
{
    std::lock_guard<std::mutex> lock(_mutex);
    this->m_acceptedList.clear();
}

void mcHubd::ConnectionInfo::clearAvailableKeyList()
{
    std::lock_guard<std::mutex> lock(_mutex);
    this->m_availableListMap.clear();
}

void mcHubd::ConnectionInfo::clearConnecteProcessMap()
{
    std::lock_guard<std::mutex> lock(_mutex);
    this->m_connectedProcessMap.clear();
}

void mcHubd::ConnectionInfo::removeAvailalbeKey(std::string& key)
{
    std::lock_guard<std::mutex> lock(_mutex);
    this->m_availableListMap.erase(key);
}


void mcHubd::ConnectionInfo::deleteConnectedProcess(pid_t pid)
{
    std::lock_guard<std::mutex> lock(_mutex);
    std::map<std::string, pid_t>::iterator itor;
    itor = this->m_connectedProcessMap.begin();
    while(itor != this->m_connectedProcessMap.end())
    {
        if(itor->second == pid)
        {
            this->m_connectedProcessMap.erase(itor++);
        }
        else
            ++itor;
    }
}

std::vector<std::string> mcHubd::ConnectionInfo::getConnectedClientKey(pid_t pid)
{
    std::lock_guard<std::mutex> lock(_mutex);
    std::vector<std::string> psVector;
    std::map<std::string, pid_t>::iterator itor;
    itor = this->m_connectedProcessMap.begin();
    while(itor != this->m_connectedProcessMap.end())
    {
        if(itor->second == pid)
            psVector.push_back(itor->first);

        ++itor;
    }

    return psVector;
}

std::vector<std::string> mcHubd::ConnectionInfo::getAcceptedList()
{
    std::lock_guard<std::mutex> lock(_mutex);
    return this->m_acceptedList;
}

std::map<std::string, key_t> mcHubd::ConnectionInfo::getAvailableList()
{
    std::lock_guard<std::mutex> lock(_mutex);
    return this->m_availableListMap;
}

std::map<std::string, pid_t> mcHubd::ConnectionInfo::getConnectedClientKeyMap()
{
    std::lock_guard<std::mutex> lock(_mutex);
    return this->m_connectedProcessMap;
}
