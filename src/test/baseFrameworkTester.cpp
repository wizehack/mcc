#include <iostream>
#include "baseFrameworkTester.h"

std::string BaseFrameworkTester::_testDataPath("");

BaseFrameworkTester::BaseFrameworkTester(){}
BaseFrameworkTester::~BaseFrameworkTester(){}

bool BaseFrameworkTester::request(TestOption* opt)
{
    if(opt == NULL)
        return false;

    if(opt->getCategory().compare("BaseFramework") == 0)
    {
        BaseFrameworkTester::_testDataPath = opt->getTestDataPath();
        return exec(opt->getTestCaseID());
    }

    if(this->m_next)
    {
        return this->m_next->request(opt);
    }

    return false;
}

void BaseFrameworkTester::registerTestCase()
{
}

