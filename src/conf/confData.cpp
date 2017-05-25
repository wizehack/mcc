#include "confData.h"

std::atomic<mcHubd::ConfData*> mcHubd::ConfData::_singleton;
std::mutex mcHubd::ConfData::_mutex;

mcHubd::ConfData::ConfData():
    m_isInitialized(false),
    m_acceptedKeyPath(),
    m_ipAddr(),
    m_port(0){}
mcHubd::ConfData::~ConfData(){}

mcHubd::ConfData* mcHubd::ConfData::getInstance() {
    mcHubd::ConfData* inst = _singleton.load(std::memory_order_relaxed);
    std::atomic_thread_fence(std::memory_order_acquire);
    if(inst == 0) {
        std::lock_guard<std::mutex> lock(_mutex);
        inst = _singleton.load(std::memory_order_relaxed);
        if(inst == 0) {
            inst = new mcHubd::ConfData();
            std::atomic_thread_fence(std::memory_order_release);
            _singleton.store(inst, std::memory_order_relaxed);
        }
    }
    return inst;
}

void mcHubd::ConfData::initialize()
{
    m_isInitialized = true;
}

void mcHubd::ConfData::setAcceptedKeyPath(std::string path)
{
    m_acceptedKeyPath = path;
}

void mcHubd::ConfData::setIPAddr(std::string ipAddr)
{
    m_ipAddr = ipAddr;
}

void mcHubd::ConfData::setPort(int port)
{
    m_port = port;
}

bool mcHubd::ConfData::isInitialized()
{
    return m_isInitialized;
}

std::string mcHubd::ConfData::getAcceptedKeyPath()
{
    return m_acceptedKeyPath;
}

std::string mcHubd::ConfData::getIPAddr()
{
    return m_ipAddr;
}

int mcHubd::ConfData::getPort()
{
    return m_port;
}
