#ifndef MESSAGE_PROCESSOR_H_
#define MESSAGE_PROCESSOR_H_

#include "manager.h"

namespace mcHubd {
    class MessageProcessor {
        public:
            MessageProcessor();
            ~MessageProcessor();
            static bool request(mcHubd::Message* msg);
    };
}

#endif /*MESSAGE_PROCESSOR_H_*/
