#include <iostream>

#include "tester.h"
#include "testSuite.h"
#include "baseFrameworkTestSuite.h"
#include "registerClientTestSuite.h"

Tester::Tester(){}
Tester::~Tester(){}

bool Tester::execute(TestOption* opt)
{
    std::cout << "test category: " << opt->getCategory() << std::endl;
    std::cout << "test case ID: " << opt->getTestCaseID() << std::endl;
    std::cout << "test data path: " << opt->getTestDataPath() << std::endl;

    TestSuite* frameworkTestSuite = new BaseFrameworkTestSuite();
    TestSuite* registerClientTestSuite = new RegisterClientTestSuite();
    frameworkTestSuite->setNext(registerClientTestSuite);

    return frameworkTestSuite->request(opt);
}
