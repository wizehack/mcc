#include <iostream>

#include "tester.h"
#include "testSuite.h"
#include "baseFrameworkTestSuite.h"
#include "registerClientTestSuite.h"
#include "registerChannelTestSuite.h"
#include "deleteChannelTestSuite.h"
#include "deleteClientTestSuite.h"

Tester::Tester(){}
Tester::~Tester(){}

bool Tester::execute(TestOption* opt)
{
    /*
    std::cout << "test category: " << opt->getCategory() << std::endl;
    std::cout << "test case ID: " << opt->getTestCaseID() << std::endl;
    std::cout << "test data path: " << opt->getTestDataPath() << std::endl;
    */

    TestSuite* frameworkTestSuite = new BaseFrameworkTestSuite();
    TestSuite* registerClientTestSuite = new RegisterClientTestSuite();
    TestSuite* registerChannelTestSuite = new RegisterChannelTestSuite();
    TestSuite* deleteChannelTestSuite = new DeleteChannelTestSuite();
    TestSuite* deleteClientTestSuite = new DeleteClientTestSuite();

    frameworkTestSuite->setNext(registerClientTestSuite);
    registerClientTestSuite->setNext(registerChannelTestSuite);
    registerChannelTestSuite->setNext(deleteChannelTestSuite);
    deleteChannelTestSuite->setNext(deleteClientTestSuite);

    return frameworkTestSuite->request(opt);
}
