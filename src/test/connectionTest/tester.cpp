#include <iostream>
#include "tester.h"
#include "../testSuite.h"
#include "registerClientTestSuite.h"
#include "registerChannelTestSuite.h"
#include "deleteChannelTestSuite.h"
#include "deleteClientTestSuite.h"
#include "requestChannelTestSuite.h"
#include "subscribeTestSuite.h"

Tester::Tester(){}
Tester::~Tester(){}

bool Tester::_execute(TestOption* opt)
{
    TestSuite* registerClientTestSuite = new RegisterClientTestSuite();
    TestSuite* registerChannelTestSuite = new RegisterChannelTestSuite();
    TestSuite* deleteChannelTestSuite = new DeleteChannelTestSuite();
    TestSuite* deleteClientTestSuite = new DeleteClientTestSuite();
    TestSuite* requestChannelTestSuite = new RequestChannelTestSuite();
    TestSuite* subscribeTestSuite = new SubscribeTestSuite();

    registerClientTestSuite->setNext(registerChannelTestSuite);
    registerChannelTestSuite->setNext(deleteChannelTestSuite);
    deleteChannelTestSuite->setNext(deleteClientTestSuite);
    deleteClientTestSuite->setNext(requestChannelTestSuite);
    requestChannelTestSuite->setNext(subscribeTestSuite);

    return registerClientTestSuite->request(opt);
}
