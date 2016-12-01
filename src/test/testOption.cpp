#include "testOption.h"
#include <string.h>
#include <stdexcept>
#include <iostream>

TestOption::TestOption(int argc, char **argv):
    m_category(),
    m_testCaseID(),
    m_testDataPath()
{
    this->parse(argc, argv);
}

TestOption::~TestOption(){}

std::string TestOption::getCategory()
{
    return this->m_category;
}

std::string TestOption::getTestDataPath()
{
    return this->m_testDataPath;
}

int TestOption::getTestCaseID()
{
    int id = 0;

    if(this->m_testCaseID.empty() == false)
    {
        try
        {
            id = std::stoi(this->m_testCaseID, nullptr);
        }
        catch (const std::invalid_argument& ia)
        {
            id = -1;
            std::cerr << "Invalid argument: " << ia.what() << '\n';
            exit(0);
        }
    }

    return id;
}

/*
 * ./test -c category -t test_case_id -d test_data_path
 */

void TestOption::parse(int argc, char **argv)
{
    const int OPT_NUMBER = 4;
    const int MAX_OPT_NUMBER = 6;
    int i = 1;

    if(argc < OPT_NUMBER)
    {
        std::cout << "argc : " << argc << std::endl;
        return;
    }

    while(i < MAX_OPT_NUMBER)
    {
        if(argv[i])
        {
            if(strcmp(argv[i], "-c") == 0)
            {
                this->m_category.assign(argv[i+1]);
            }
            else if (strcmp(argv[i], "-t") == 0)
            {
                this->m_testCaseID.assign(argv[i+1]);
            }
            else if (strcmp(argv[i], "-d") == 0)
            {
                this->m_testDataPath.assign(argv[i+1]);
            }
            else
            {
                std::cout << "argv " << i << " : " << argv[i] << std::endl;
            }

            i = i + 2;
        }
        else
            break;
    }
}
