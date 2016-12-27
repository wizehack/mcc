#ifndef BASE_FRAMEWORK_TESTER_H_
#define BASE_FRAMEWORK_TESTER_H_

#include "testSuite.h"
#include "../manager.h"
#include "../mediator.h"

class DummyManager : public mcHubd::Manager {
    public:
        DummyManager(mcHubd::Mediator* mediator, std::string role);
        ~DummyManager();

        void create(mcHubd::Contract** contract){};
        void add(mcHubd::Contract** contract){};
        void remove(mcHubd::Contract** contract){};
        void get(mcHubd::Contract** contract){};
};

class DummyMediator : public mcHubd::Mediator {
    public:
        DummyMediator();
        ~DummyMediator();
        void appendManager(std::shared_ptr<mcHubd::Manager> manager){};
        void removeManager(std::shared_ptr<mcHubd::Manager> manager){};

        void getNewChannel(mcHubd::Contract** pContract){};
        void registerNewChannel(mcHubd::Contract** pContract){};
        void deleteClient(mcHubd::Contract** pContract){};
        void deleteChannel(mcHubd::Contract** pContract){};

        void notify(mcHubd::Contract* contract, mcHubd::CONTRACTREASON reason){};
};

class BaseFrameworkTestSuite : public TestSuite{
    public:
        BaseFrameworkTestSuite();
        ~BaseFrameworkTestSuite();
        bool request(TestOption* opt);

        static bool _testSetupAcceptedList();
        static bool _testCheckAcceptedClientKey();
        static bool _testEmptyWaitingTask();
        static bool _testClearAcceptedList();
        static bool _testInvalidAcceptedList();

    protected:
        void registerTestCase();

    private:
        static std::string _testDataPath;
};

#endif /*BASE_FRAMEWORK_TESTER_H_*/
