#include <iostream>
#include "registerClientTestSuite.h"
#include "../registerClientHandler.h"
#include "../mcHubType.h"
#include "../message.h"
#include "../taskSet.h"
#include "../manager.h"
#include "../clientManager.h"
#include "../mediator.h"
#include "../channelStatusMediator.h"

std::string RegisterClientTestSuite::_testDataPath("");

RegisterClientTestSuite::RegisterClientTestSuite() : TestSuite(){}
RegisterClientTestSuite::~RegisterClientTestSuite(){}

bool RegisterClientTestSuite::request(TestOption* opt)
{
    if(opt == NULL)
        return false;

    if(opt->getCategory().compare("RegisterClient") == 0)
    {
        RegisterClientTestSuite::_testDataPath = opt->getTestDataPath();
        this->registerTestCase();
        return this->exec(opt->getTestCaseID());
    }

    if(this->m_next)
    {
        return this->m_next->request(opt);
    }

    return false;
}

void RegisterClientTestSuite::registerTestCase()
{
    mcHubd::Mediator* mediator = new mcHubd::ChannelStatusMediator();
    mcHubd::Manager* manager = new mcHubd::ClientManager(mediator);
    manager->setUpAcceptedList(RegisterClientTestSuite::_testDataPath);

    this->add(1, RegisterClientTestSuite::_testRegisterClientWithOneKey);
    this->add(2, RegisterClientTestSuite::_testRegisterClientWithManyKey);
    this->add(3, RegisterClientTestSuite::_testRegisterClientWithMAXKey);
    this->add(4, RegisterClientTestSuite::_testRegisterClientWitUnlimitedKey);
    this->add(5, RegisterClientTestSuite::_testRegisterClientWitUnacceptedKey);

    delete mediator;
    delete manager;
}

bool RegisterClientTestSuite::_testRegisterClientWithOneKey()
{
    mcHubd::RegisterClientHandler regCliHandler;
    std::shared_ptr<mcHubd::Message> msg = std::make_shared<mcHubd::Message>(mcHubd::REQ_REG_CLIENT);
    std::string body;
    std::string key("com.mchannel.test.t1");
    body.assign("{\"pid\": 1212, \"psName\": \"test\", \"keyList\":");
    body.append("[\"com.mchannel.test.t1\"]");
    body.append("}");
    msg->setBody(body);
    regCliHandler.request(msg);

    if(mcHubd::TaskSet::getInstance()->isWaitingTask(key) == false)
       return false;

    return true;
}

bool RegisterClientTestSuite::_testRegisterClientWithManyKey()
{
    mcHubd::RegisterClientHandler regCliHandler;
    std::shared_ptr<mcHubd::Message> msg = std::make_shared<mcHubd::Message>(mcHubd::REQ_REG_CLIENT);
    std::string body;
    std::string foo1("com.mchannel.foo.f1");
    std::string foo2("com.mchannel.foo.f2");
    std::string bar1("com.mchannel.bar.b1");
    std::string bar2("com.mchannel.bar.b2");

    body.assign("{\"pid\": 1212, \"psName\": \"foobar\",\"keyList\": ");
    body.append("[");
    body.append("\"com.mchannel.foo.f1\", ");
    body.append("\"com.mchannel.foo.f2\", ");
    body.append("\"com.mchannel.bar.b1\", ");
    body.append("\"com.mchannel.bar.b2\" " );
    body.append("]}");
    msg->setBody(body);
    regCliHandler.request(msg);

    if(mcHubd::TaskSet::getInstance()->isWaitingTask(foo1) == false)
       return false;

    if(mcHubd::TaskSet::getInstance()->isWaitingTask(foo2) == false)
       return false;

    if(mcHubd::TaskSet::getInstance()->isWaitingTask(bar1) == false)
       return false;

    if(mcHubd::TaskSet::getInstance()->isWaitingTask(bar2) == false)
       return false;

    return true;
}

bool RegisterClientTestSuite::_testRegisterClientWithMAXKey()
{
    mcHubd::RegisterClientHandler regCliHandler;
    std::shared_ptr<mcHubd::Message> msg = std::make_shared<mcHubd::Message>(mcHubd::REQ_REG_CLIENT);
    std::string body;
    std::string t1("com.mchannel.test.t1");
    std::string t2("com.mchannel.test.t2");
    std::string t3("com.mchannel.test.t3");
    std::string t4("com.mchannel.test.t4");
    std::string t5("com.mchannel.test.t5");
    std::string t6("com.mchannel.test.t6");
    std::string t7("com.mchannel.test.t7");
    std::string t8("com.mchannel.test.t8");
    std::string t9("com.mchannel.test.t9");
    std::string t10("com.mchannel.test.t10");
    std::string t11("com.mchannel.test.t11");
    std::string t12("com.mchannel.test.t12");
    std::string t13("com.mchannel.test.t13");

    body.assign("{\"pid\": 1212, \"psName\": \"test\",\"keyList\":");
    body.append("[");
    body.append("\"com.mchannel.test.t1\",");
    body.append("\"com.mchannel.test.t2\",");
    body.append("\"com.mchannel.test.t3\", ");
    body.append("\"com.mchannel.test.t4\", ");
    body.append("\"com.mchannel.test.t5\", ");
    body.append("\"com.mchannel.test.t6\", ");
    body.append("\"com.mchannel.test.t7\", ");
    body.append("\"com.mchannel.test.t8\", ");
    body.append("\"com.mchannel.test.t9\", ");
    body.append("\"com.mchannel.test.t10\", ");
    body.append("\"com.mchannel.test.t11\", ");
    body.append("\"com.mchannel.test.t12\", ");
    body.append("\"com.mchannel.test.t13\"");
    body.append("]");
    body.append("}");
    msg->setBody(body);
    regCliHandler.request(msg);

    if(mcHubd::TaskSet::getInstance()->isWaitingTask(t1))
       return false;
    if(mcHubd::TaskSet::getInstance()->isWaitingTask(t2))
       return false;
    if(mcHubd::TaskSet::getInstance()->isWaitingTask(t3))
       return false;
    if(mcHubd::TaskSet::getInstance()->isWaitingTask(t4))
       return false;
    if(mcHubd::TaskSet::getInstance()->isWaitingTask(t5))
       return false;
    if(mcHubd::TaskSet::getInstance()->isWaitingTask(t6))
       return false;
    if(mcHubd::TaskSet::getInstance()->isWaitingTask(t7))
       return false;
    if(mcHubd::TaskSet::getInstance()->isWaitingTask(t8))
       return false;
    if(mcHubd::TaskSet::getInstance()->isWaitingTask(t9))
       return false;
    if(mcHubd::TaskSet::getInstance()->isWaitingTask(t10))
       return false;
    if(mcHubd::TaskSet::getInstance()->isWaitingTask(t11))
       return false;
    if(mcHubd::TaskSet::getInstance()->isWaitingTask(t12))
       return false;
    if(mcHubd::TaskSet::getInstance()->isWaitingTask(t13))
       return false;

    return true;
}

bool RegisterClientTestSuite::_testRegisterClientWitUnlimitedKey()
{
    mcHubd::RegisterClientHandler regCliHandler;
    std::shared_ptr<mcHubd::Message> msg = std::make_shared<mcHubd::Message>(mcHubd::REQ_REG_CLIENT);
    std::string body;
    std::string t1("com.mchannel.test.t1");
    std::string t2("com.mchannel.test.t2");
    std::string t3("com.mchannel.test.t3");
    std::string t4("com.mchannel.test.t4");
    std::string t5("com.mchannel.test.t5");
    std::string t6("com.mchannel.test.t6");
    std::string t7("com.mchannel.test.t7");
    std::string t8("com.mchannel.test.t8");
    std::string t9("com.mchannel.test.t9");
    std::string t10("com.mchannel.test.t10");

    body.assign("{\"pid\": 1212, \"psName\": \"k\", \"keyList\": ");
    body.append("[");
    body.append("\"com.mchannel.test.t1\",");
    body.append("\"com.mchannel.test.t2\",");
    body.append("\"com.mchannel.test.t3\",");
    body.append("\"com.mchannel.test.t4\",");
    body.append("\"com.mchannel.test.t5\",");
    body.append("\"com.mchannel.test.t6\",");
    body.append("\"com.mchannel.test.t7\",");
    body.append("\"com.mchannel.test.t8\",");
    body.append("\"com.mchannel.test.t9\",");
    body.append("\"com.mchannel.test.t10\"");
    body.append("]");
    body.append("}");
    msg->setBody(body);
    regCliHandler.request(msg);

    if(mcHubd::TaskSet::getInstance()->isWaitingTask(t1) == false)
       return false;
    if(mcHubd::TaskSet::getInstance()->isWaitingTask(t2) == false)
       return false;
    if(mcHubd::TaskSet::getInstance()->isWaitingTask(t3) == false)
       return false;
    if(mcHubd::TaskSet::getInstance()->isWaitingTask(t4) == false)
       return false;
    if(mcHubd::TaskSet::getInstance()->isWaitingTask(t5) == false)
       return false;
    if(mcHubd::TaskSet::getInstance()->isWaitingTask(t6) == false)
       return false;
    if(mcHubd::TaskSet::getInstance()->isWaitingTask(t7) == false)
       return false;
    if(mcHubd::TaskSet::getInstance()->isWaitingTask(t8) == false)
       return false;
    if(mcHubd::TaskSet::getInstance()->isWaitingTask(t9) == false)
       return false;
    if(mcHubd::TaskSet::getInstance()->isWaitingTask(t10) == false)
       return false;

    return true;
}

bool RegisterClientTestSuite::_testRegisterClientWitUnacceptedKey()
{
    mcHubd::RegisterClientHandler regCliHandler;
    std::shared_ptr<mcHubd::Message> msg = std::make_shared<mcHubd::Message>(mcHubd::REQ_REG_CLIENT);
    std::string body;
    std::string u1("com.mchannel.unaccepted.u1");

    body.assign("{\"pid\": 1212, \"psName\": \"unaccepted\",\"keyList\": ");
    body.append("[");
    body.append("\"com.mchannel.unaccepted.u1\"");
    body.append("]");
    body.append("}");
    msg->setBody(body);
    regCliHandler.request(msg);

    if(mcHubd::TaskSet::getInstance()->isWaitingTask(u1))
       return false;

    return true;
}
