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
//        std::cout << "######### START TEST #########" << std::endl;

        tester = new Tester();
        isSuccess = tester->execute(opt);

        if(isSuccess)
            std::cout << opt->getCategory() << " " << opt->getTestCaseID() << ": Success" << std::endl;
        else
            std::cout << opt->getCategory() << " " << opt->getTestCaseID() << ": Failure" << std::endl;

        delete tester;
        delete opt;

//        std::cout << "########## END TEST ##########" << std::endl;
    }
    else
        std::cout << "NOT tested" << std::endl;

    return 0;
}
