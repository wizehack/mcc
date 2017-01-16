#include "messageQueue.h"
#include "message.h"
#include "messageHandler.h"
#include "registerChannelHandler.h"
#include "registerClientHandler.h"
#include "deleteChannelHandler.h"
#include "deleteClientHandler.h"

std::atomic<mcHubd::MessageQueue*> mcHubd::MessageQueue::_singleton;
std::mutex mcHubd::MessageQueue::_mutex;

mcHubd::MessageQueue::MessageQueue():m_reqMessageQueue(g_async_queue_new()){}

mcHubd::MessageQueue::~MessageQueue(){}

mcHubd::MessageQueue* mcHubd::MessageQueue::getInstance() {
	mcHubd::MessageQueue* inst = _singleton.load(std::memory_order_relaxed);
	std::atomic_thread_fence(std::memory_order_acquire);
	if(inst == 0) {
		std::lock_guard<std::mutex> lock(_mutex);
		inst = _singleton.load(std::memory_order_relaxed);
		if(inst == 0) {
			inst = new mcHubd::MessageQueue();
			std::atomic_thread_fence(std::memory_order_release);
			_singleton.store(inst, std::memory_order_relaxed);
		}
	}
	return inst;
}

GAsyncQueue* mcHubd::MessageQueue::getReqMessageQueue()
{
    return this->m_reqMessageQueue;
}

gpointer mcHubd::MessageQueue::_msgProcessor(void* data)
{
    mcHubd::MessageHandler* regClientHandler = new mcHubd::RegisterClientHandler();
    mcHubd::MessageHandler* regChannelHandler = new mcHubd::RegisterChannelHandler();
    mcHubd::MessageHandler* delClientHandler = new mcHubd::DeleteClientHandler();
    mcHubd::MessageHandler* delChannelHandler = new mcHubd::DeleteChannelHandler();

    while(true)
    {
        gpointer item = g_async_queue_pop(mcHubd::MessageQueue::getInstance()->getReqMessageQueue());
        mcHubd::Message* message = static_cast<mcHubd::Message*>(item);

        if(message)
        {
            regClientHandler->setNext(regChannelHandler);
            regChannelHandler->setNext(delClientHandler);
            delClientHandler->setNext(delChannelHandler);
            regClientHandler->request(message);
        }
    }

    return NULL;
}

