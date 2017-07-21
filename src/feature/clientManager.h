#ifndef CLIENT_MANAGER_H_
#define CLIENT_MANAGER_H_

#include "connectionInfo.h"
#include "manager.h"
#include "mediator.h"

namespace mcHubd {
    class ClientManager : public Manager {
        public:
            ClientManager(mcHubd::Mediator* mediator);
            ~ClientManager();

            void create(mcHubd::Contract** contract);
            void add(mcHubd::Contract** contract);
            void remove(mcHubd::Contract** contract);
            void get(mcHubd::Contract** contract);

        private:
            void add(std::string cKey, int numId);
            bool remove(pid_t pid, std::string psName);

        private:
            ClientManager();
            ClientManager(const mcHubd::ClientManager&);
            ClientManager& operator=(const mcHubd::ClientManager& rhs);

        private:
            mcHubd::ConnectionInfo* m_cInfo;
    };
}

#endif /*CLIENT_MANAGER_H_*/
