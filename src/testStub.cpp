#include "testStub.h"

std::atomic<mcHubd::TestStub*> mcHubd::TestStub::_singleton;
std::mutex mcHubd::TestStub::_mutex;

mcHubd::TestStub::TestStub():
    m_respMsgMap(),
    m_subscribeMsgMap(),
    m_id(0),
    m_sid(0){}
mcHubd::TestStub::~TestStub(){}

mcHubd::TestStub* mcHubd::TestStub::getInstance() {
    mcHubd::TestStub* inst = _singleton.load(std::memory_order_relaxed);
    std::atomic_thread_fence(std::memory_order_acquire);
    if(inst == 0) {
        std::lock_guard<std::mutex> lock(_mutex);
        inst = _singleton.load(std::memory_order_relaxed);
        if(inst == 0) {
            inst = new mcHubd::TestStub();
            std::atomic_thread_fence(std::memory_order_release);
            _singleton.store(inst, std::memory_order_relaxed);
        }
    }
    return inst;
}

void mcHubd::TestStub::addRespMsg(std::string msg)
{
    std::lock_guard<std::mutex> lock(_mutex);
    this->m_respMsgMap.insert(std::pair<int, std::string>(this->m_id, msg));
    this->m_id++;
}

std::string mcHubd::TestStub::getRespMsg(int id)
{
    std::lock_guard<std::mutex> lock(_mutex);

    std::string msg;
    std::map<int, std::string>::iterator it;
    it = this->m_respMsgMap.find(id);

    if(it != this->m_respMsgMap.end())
    {
        msg = it->second;
    }

    return msg;
}

void mcHubd::TestStub::addSubscribeMsg(std::string msg)
{
    std::lock_guard<std::mutex> lock(_mutex);
    this->m_subscribeMsgMap.insert(std::pair<int, std::string>(this->m_sid, msg));
    this->m_sid++;
}

std::string mcHubd::TestStub::getSubscribepMsg(int id)
{
    std::lock_guard<std::mutex> lock(_mutex);

    std::string msg;
    std::map<int, std::string>::iterator it;
    it = this->m_subscribeMsgMap.find(id);

    if(it != this->m_subscribeMsgMap.end())
    {
        msg = it->second;
    }

    return msg;
}
