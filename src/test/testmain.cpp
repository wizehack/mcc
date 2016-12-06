#include <iostream>
#include "tester.h"
#include "testOption.h"
#include "tester.h"

int main(int argc, char **argv)
{
    TestOption* opt = NULL;
    Tester* tester = NULL;
    std::string category;
    std::string dataPath;
    bool isSuccess = false;

    opt = new TestOption(argc, argv);

    if(opt)
    {
        tester = new Tester();
        isSuccess = tester->execute(opt);

        if(isSuccess)
            std::cout << opt->getCategory() << " " << opt->getTestCaseID() << " : " << "Result: Success" << std::endl;
        else
            std::cout << opt->getCategory() << " " << opt->getTestCaseID() << " : " << "Result: Failure" << std::endl;

        delete tester;
        delete opt;
    }
    else
        std::cout << "NOT tested" << std::endl;

    return 0;
}
