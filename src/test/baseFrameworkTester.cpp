#include <iostream>
#include "baseFrameworkTester.h"

std::string BaseFrameworkTester::_testDataPath("");

BaseFrameworkTester::BaseFrameworkTester() : Tester(){}
BaseFrameworkTester::~BaseFrameworkTester(){}

bool BaseFrameworkTester::request(TestOption* opt)
{
    if(opt == NULL)
        return false;

    if(opt->getCategory().compare("BaseFramework") == 0)
    {
        this->registerTestCase();
        BaseFrameworkTester::_testDataPath = opt->getTestDataPath();
        return this->exec(opt->getTestCaseID());
    }

    if(this->m_next)
    {
        return this->m_next->request(opt);
    }

    return false;
}

void BaseFrameworkTester::registerTestCase()
{
    this->add(1, BaseFrameworkTester::testSetupAcceptedList);
    this->add(2, BaseFrameworkTester::testCheckAcceptedClientKey);
}

bool BaseFrameworkTester::testSetupAcceptedList()
{
    mcHubd::Mediator* mediator = new DummyMediator();
    mcHubd::Manager* manager = new DummyManager(mediator, "Manager");

    manager->setUpAcceptedList(BaseFrameworkTester::_testDataPath);
    return manager->hasAcceptedList();

    delete mediator;
    delete manager;
}

bool BaseFrameworkTester::testCheckAcceptedClientKey()
{
    bool bResult = true;
    mcHubd::Mediator* mediator = new DummyMediator();
    mcHubd::Manager* manager = new DummyManager(mediator, "Manager");

    std::string t1("com.mchannel.test.t1");
    std::string t2("com.mchannel.test.t2");
    std::string t3("com.mchannel.test.t3");
    std::string t4("com.mchannel.test.t4");
    std::string unAccepted("com.mchannel.test.u1");

    manager->setUpAcceptedList(BaseFrameworkTester::_testDataPath);

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

DummyManager::DummyManager(mcHubd::Mediator* mediator, std::string role) :
    Manager(mediator, role){}
DummyManager::~DummyManager(){}

DummyMediator::DummyMediator(){}
DummyMediator::~DummyMediator(){}
