#include <iostream>
#include "tester.h"
#include "testOption.h"
#include "testManager.h"

int main(int argc, char **argv)
{
    TestOption* opt = NULL;
    TestManager* tMgr = NULL;
    std::string category;
    std::string dataPath;
    bool isSuccess = false;

    opt = new TestOption(argc, argv);

    if(opt)
    {
        std::cout << "######### START TEST #########" << std::endl;

        tMgr = new TestManager();
        isSuccess = tMgr->execute(opt);

        if(isSuccess)
            std::cout << "Result: Success" << std::endl;
        else
            std::cout << "Result: Failure" << std::endl;

        delete tMgr;
        delete opt;

        std::cout << "########## END TEST ##########" << std::endl;
    }
    else
        std::cout << "NOT tested" << std::endl;

    return 0;
}
