#ifndef CHANNEL_MANAGER_H_
#define CHANNEL_MANAGER_H_

#include "connectionInfo.h"
#include "taskSet.h"
#include "manager.h"
#include "mediator.h"

namespace mcHubd {
    class ChannelManager : public Manager{
        public:
            ChannelManager(mcHubd::Mediator* mediator);
            ~ChannelManager();

            void create(mcHubd::Contract** contract);
            void add(mcHubd::Contract** contract);
            void remove(mcHubd::Contract** contract);
            void get(mcHubd::Contract** contract);

        private:
            ChannelManager();
            ChannelManager(const mcHubd::ChannelManager&);
            ChannelManager& operator=(const mcHubd::ChannelManager& rhs);

            key_t createNewChannel(std::string name, int id);
            key_t makeChannelNumber(std::string& cKey, int id);
            void removeChannel(key_t ch);

        private:
            mcHubd::TaskSet* m_tSet;
            const int MAX;
    };
}

#endif /*CHANNEL_MANAGER_H_*/
