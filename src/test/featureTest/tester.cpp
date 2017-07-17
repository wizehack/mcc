#include <iostream>

#include "tester.h"
#include "../testSuite.h"
#include "baseFrameworkTestSuite.h"
#include "registerClientTestSuite.h"
#include "registerChannelTestSuite.h"
#include "deleteChannelTestSuite.h"
#include "deleteClientTestSuite.h"
#include "subscriptionTestSuite.h"
#include "requestChannelTestSuite.h"
#include "undefinedTypeTestSuite.h"

Tester::Tester(){}
Tester::~Tester(){}

bool Tester::_execute(TestOption* opt)
{
    TestSuite* frameworkTestSuite = new BaseFrameworkTestSuite();
    TestSuite* registerClientTestSuite = new RegisterClientTestSuite();
    TestSuite* registerChannelTestSuite = new RegisterChannelTestSuite();
    TestSuite* deleteChannelTestSuite = new DeleteChannelTestSuite();
    TestSuite* deleteClientTestSuite = new DeleteClientTestSuite();
    TestSuite* subscriptionTestSuite = new SubscriptionTestSuite();
    TestSuite* requestChannelTestSuite = new RequestChannelTestSuite();
    TestSuite* undefinedTypeTestSuite = new UndefinedTypeTestSuite();

    frameworkTestSuite->setNext(registerClientTestSuite);
    registerClientTestSuite->setNext(registerChannelTestSuite);
    registerChannelTestSuite->setNext(deleteChannelTestSuite);
    deleteChannelTestSuite->setNext(deleteClientTestSuite);
    deleteClientTestSuite->setNext(subscriptionTestSuite);
    subscriptionTestSuite->setNext(requestChannelTestSuite);
    requestChannelTestSuite->setNext(undefinedTypeTestSuite);

    return frameworkTestSuite->request(opt);
}
