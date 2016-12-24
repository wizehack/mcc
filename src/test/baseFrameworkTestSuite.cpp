#include <iostream>
#include "baseFrameworkTestSuite.h"
#include "../taskSet.h"

std::string BaseFrameworkTestSuite::_testDataPath("");

BaseFrameworkTestSuite::BaseFrameworkTestSuite() : TestSuite(){}
BaseFrameworkTestSuite::~BaseFrameworkTestSuite(){}

bool BaseFrameworkTestSuite::request(TestOption* opt)
{
    if(opt == NULL)
        return false;

    if(opt->getCategory().compare("BaseFramework") == 0)
    {
        this->registerTestCase();
        BaseFrameworkTestSuite::_testDataPath = opt->getTestDataPath();
        return this->exec(opt->getTestCaseID());
    }

    if(this->m_next)
    {
        return this->m_next->request(opt);
    }

    return false;
}

void BaseFrameworkTestSuite::registerTestCase()
{
    this->add(1, BaseFrameworkTestSuite::_testSetupAcceptedList);
    this->add(2, BaseFrameworkTestSuite::_testCheckAcceptedClientKey);
    this->add(3, BaseFrameworkTestSuite::_testEmptyWaitingTask);
}

bool BaseFrameworkTestSuite::_testSetupAcceptedList()
{
    mcHubd::Mediator* mediator = new DummyMediator();
    mcHubd::Manager* manager = new DummyManager(mediator, "Manager");

    manager->_setUpAcceptedList(BaseFrameworkTestSuite::_testDataPath);
    bool hasList = manager->hasAcceptedList();

    delete mediator;
    delete manager;

    return hasList;
}

bool BaseFrameworkTestSuite::_testCheckAcceptedClientKey()
{
    bool bResult = true;
    mcHubd::Mediator* mediator = new DummyMediator();
    mcHubd::Manager* manager = new DummyManager(mediator, "Manager");

    std::string t1("com.mchannel.test.t1");
    std::string t2("com.mchannel.test.t2");
    std::string t3("com.mchannel.test.t3");
    std::string t4("com.mchannel.test.t4");
    std::string unAccepted("com.mchannel.test.u1");

    manager->_setUpAcceptedList(BaseFrameworkTestSuite::_testDataPath);

    if(manager->isAccepted(t1) == false)
        bResult = false;

    if(manager->isAccepted(t2) == false)
        bResult = false;

    if(manager->isAccepted(t3) == false)
        bResult = false;

    if(manager->isAccepted(t4) == false)
        bResult = false;

    if(manager->isAccepted(unAccepted))
        bResult = false;

    return bResult;
}

bool BaseFrameworkTestSuite::_testEmptyWaitingTask()
{
    std::string key1("com.mchannel.test.t1");
    std::string key2("com.mchannel.test.t13");
    std::string key3("com.mchannel.test.b2");

    if(mcHubd::TaskSet::getInstance()->isWaitingTask(key1))
       return false;

    if(mcHubd::TaskSet::getInstance()->isWaitingTask(key2))
        return false;

    if(mcHubd::TaskSet::getInstance()->isWaitingTask(key3))
        return false;

    return true;
}

DummyManager::DummyManager(mcHubd::Mediator* mediator, std::string role) :
    Manager(mediator, role){}
DummyManager::~DummyManager(){}

DummyMediator::DummyMediator(){}
DummyMediator::~DummyMediator(){}
