#ifndef CHANNEL_STATUS_MEDIATOR_H_
#define CHANNEL_STATUS_MEDIATOR_H_

#include "mcHubType.h"
#include "mediator.h"
#include "manager.h"

namespace mcHubd {
    class ChannelStatusMediator : public Mediator {
        public:
            ChannelStatusMediator();
            ~ChannelStatusMediator();
            void appendManager(std::shared_ptr<mcHubd::Manager> manager);
            void removeManager(std::shared_ptr<mcHubd::Manager> manager);

            void getNewChannel(mcHubd::Contract** pContract);
            void registerNewChannel(mcHubd::Contract** pContract);
            void deleteClient(mcHubd::Contract** pContract);
            void deleteChannel(mcHubd::Contract** pContract);

            void notify(mcHubd::Contract* contract, mcHubd::CONTRACTREASON reason);

        private:
            mcHubd::RESPCODE checkRegisterStatus(std::string& cKey);
            mcHubd::RESPCODE checkCreateStatus(std::string& cKey);
            bool createNewChannel(mcHubd::Contract** pContract);
            void notifyCreateChannel(std::string cKey, key_t channel);
            void deleteClient(pid_t pid);
            void deleteChannel(std::string cKey);

        private:
            mcHubd::Manager* getManager(std::string& role);

        private:
//            mcHubd::ChannelSubscriberData* m_chSubscriberData;
            std::list<std::shared_ptr<mcHubd::Manager>> m_mgrList;
    };
}

#endif /*CHANNEL_STATUS_MEDIATOR_H_*/
