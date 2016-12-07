#ifndef MANAGER_H_
#define MANAGER_H_

#include "mcHubType.h"
#include "mediator.h"
#include "connectionInfo.h"

namespace mcHubd {
    class Contract;
    class Manager {
        public:
            Manager(mcHubd::Mediator* mediator, std::string role);
            virtual ~Manager();
            std::string getRole();

            /* configuratoin */
            void setUpAcceptedList(std::string& acceptedKeyFilePath);
            void clearAcceptedList(std::string& acceptedKeyFilePath);

            /* check */
            bool hasAcceptedList();
            bool isAccepted(std::string& key);
            bool isAvailable(std::string& key);

            /* create/add/delete */
            virtual void create(mcHubd::Contract** contract) = 0;
            virtual void add(mcHubd::Contract** contract) = 0;
            virtual void remove(mcHubd::Contract** contract) = 0;
            virtual void get(mcHubd::Contract** contract) = 0;

        protected:
            mcHubd::Mediator* m_mediator;
            std::string m_role;
            mcHubd::ConnectionInfo* m_cInfo;

        private:
            Manager();
            Manager(const mcHubd::Manager&);
            Manager& operator=(const mcHubd::Manager& rhs);
    };
}

#endif /*MANAGER_H_*/
