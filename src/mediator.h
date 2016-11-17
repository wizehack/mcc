#ifndef MEDIATOR_H_
#define MEDIATOR_H_

#include "mcHubType.h"
#include "contract.h"

namespace mcHubd {
    class Manager;
    class Mediator {
        public:
            Mediator();
            virtual ~Mediator();
            virtual void appendManager(std::shared_ptr<mcHubd::Manager> manager) = 0;
            virtual void removeManager(std::shared_ptr<mcHubd::Manager> manager) = 0;

            virtual void getNewChannel(mcHubd::Contract** pContract) = 0;
            virtual void registerNewChannel(mcHubd::Contract** pContract) = 0;
            virtual void deleteClient(mcHubd::Contract** pContract) = 0;
            virtual void deleteChannel(mcHubd::Contract** pContract) = 0;

            virtual void notify(mcHubd::Contract* contract, mcHubd::CONTRACTREASON reason) = 0;
    };
}

#endif /* MEDIATOR_H_ */
