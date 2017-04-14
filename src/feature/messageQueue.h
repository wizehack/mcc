#ifndef MESSAGE_QUEUE_H_
#define MESSAGE_QUEUE_H_

#include <glib-2.0/glib.h>
#include "mcHubType.h"

namespace mcHubd {
    class MessageQueue {
        public:
            ~MessageQueue();
            static MessageQueue* getInstance();
            GAsyncQueue* getReqMessageQueue();

            static gpointer _msgProcessor(void* data);

        private:
            MessageQueue();
            MessageQueue(const MessageQueue&);
            MessageQueue operator=(const MessageQueue&);

        private:
            GAsyncQueue* m_reqMessageQueue;
            static std::atomic<MessageQueue*> _singleton;
            static std::mutex _mutex;
    };
}

#endif /* MESSAGE_QUEUE_H_ */
