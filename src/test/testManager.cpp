#include <iostream>

#include "testManager.h"
#include "tester.h"
#include "baseFrameworkTester.h"

TestManager::TestManager(){}
TestManager::~TestManager(){}

bool TestManager::execute(TestOption* opt)
{
    std::cout << "test category: " << opt->getCategory() << std::endl;
    std::cout << "test case ID: " << opt->getTestCaseID() << std::endl;
    std::cout << "test data path: " << opt->getTestDataPath() << std::endl;

    Tester* fTester = new BaseFrameworkTester();
//    Tester* rcTester = new RegisterClientFeatureTester();
//    fTester->setNext(rcTester);

    return fTester->request(opt);
}
