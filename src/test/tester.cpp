#include <iostream>

#include "tester.h"
#include "testSuite.h"
#include "baseFrameworkTestSuite.h"
#include "registerClientTestSuite.h"
#include "registerChannelTestSuite.h"
#include "deleteChannelTestSuite.h"
#include "deleteClientTestSuite.h"
#include "messageQueueTestSuite.h"
#include "subscriptionTestSuite.h"

Tester::Tester(){}
Tester::~Tester(){}

bool Tester::_execute(TestOption* opt)
{
    TestSuite* frameworkTestSuite = new BaseFrameworkTestSuite();
    TestSuite* registerClientTestSuite = new RegisterClientTestSuite();
    TestSuite* registerChannelTestSuite = new RegisterChannelTestSuite();
    TestSuite* deleteChannelTestSuite = new DeleteChannelTestSuite();
    TestSuite* deleteClientTestSuite = new DeleteClientTestSuite();
    TestSuite* messageQueueTestSuite = new MessageQueueTestSuite();
    TestSuite* subscriptionTestSuite = new SubscriptionTestSuite();

    frameworkTestSuite->setNext(registerClientTestSuite);
    registerClientTestSuite->setNext(registerChannelTestSuite);
    registerChannelTestSuite->setNext(deleteChannelTestSuite);
    deleteChannelTestSuite->setNext(deleteClientTestSuite);
    deleteClientTestSuite->setNext(messageQueueTestSuite);
    messageQueueTestSuite->setNext(subscriptionTestSuite);

    return frameworkTestSuite->request(opt);
}
