#include <iostream>

#include "tester.h"
#include "../testSuite.h"
#include "configOptTestSuite.h"

Tester::Tester(){}
Tester::~Tester(){}

bool Tester::_execute(TestOption* opt)
{
    TestSuite* confOptTestSuite = new ConfigOptionTestSuite();
    return confOptTestSuite->request(opt);
}
