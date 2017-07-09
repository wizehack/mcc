#include <iostream>
#include "connection/connectionManager.h"
#include "tester.h"
#include "../testOption.h"

int main(int argc, char **argv)
{
    TestOption* opt = NULL;

    opt = new TestOption(argc, argv);

    if(opt)
    {
        if(Tester::_execute(opt))
            std::cout << "[TEST] " << opt->getCategory() << " " << opt->getTestCaseID() << " : " << "Result: Success" << std::endl;
        else
            std::cout << "[TEST] " << opt->getCategory() << " " << opt->getTestCaseID() << " : " << "Result: Failure" << std::endl;

        delete opt;
    }
    else
        std::cout << "NOT tested" << std::endl;

    return 0;
}
