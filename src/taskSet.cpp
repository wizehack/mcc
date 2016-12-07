#include <iostream>
#include <algorithm>
#include "taskSet.h"

std::atomic<mcHubd::TaskSet*> mcHubd::TaskSet::_singleton;
std::mutex mcHubd::TaskSet::_mutex;

mcHubd::TaskSet::TaskSet():
    m_waitToReadyList(){}
mcHubd::TaskSet::~TaskSet(){}

mcHubd::TaskSet* mcHubd::TaskSet::getInstance() {
    mcHubd::TaskSet* inst = _singleton.load(std::memory_order_relaxed);
    std::atomic_thread_fence(std::memory_order_acquire);
    if(inst == 0) {
        std::lock_guard<std::mutex> lock(_mutex);
        inst = _singleton.load(std::memory_order_relaxed);
        if(inst == 0) {
            inst = new mcHubd::TaskSet();
            std::atomic_thread_fence(std::memory_order_release);
            _singleton.store(inst, std::memory_order_relaxed);
        }
    }
    return inst;
}

void mcHubd::TaskSet::addWaitToReadyTask(std::string cKey)
{
    std::lock_guard<std::mutex> lock(_mutex);
    std::list <std::string>::iterator itor;
    itor = std::find(this->m_waitToReadyList.begin(), this->m_waitToReadyList.end(), cKey);

    if(itor == this->m_waitToReadyList.end())
        this->m_waitToReadyList.push_back(cKey);
}

void mcHubd::TaskSet::rmWaitToReady(std::string& cKey)
{
    std::lock_guard<std::mutex> lock(_mutex);
    std::list <std::string>::iterator itor;
    itor = std::find(this->m_waitToReadyList.begin(), this->m_waitToReadyList.end(), cKey);

    if(itor != this->m_waitToReadyList.end())
        this->m_waitToReadyList.erase(itor++);
}

bool mcHubd::TaskSet::isWaitingTask(std::string& cKey)
{
    std::lock_guard<std::mutex> lock(_mutex);
    std::list <std::string>::iterator itor;
    itor = std::find(this->m_waitToReadyList.begin(), this->m_waitToReadyList.end(), cKey);

    if(itor == this->m_waitToReadyList.end())
        return false;

    return true;
}

void mcHubd::TaskSet::clearWaitingTask()
{
    std::lock_guard<std::mutex> lock(_mutex);
    this->m_waitToReadyList.clear();
}
