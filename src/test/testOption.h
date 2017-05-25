#ifndef TEST_OPTION__H
#define TEST_OPTION__H

#include <string>

class TestOption {
    public:
        TestOption(int argc, char **argv);
        ~TestOption();

        std::string& getCategory();
        std::string& getTestDataPath();
        std::string& getConfigPath();
        std::string& getExpectedOut();
        int getTestCaseID();

    private:
        TestOption();
        TestOption(const TestOption&);
        void operator=(const TestOption&);

        void parse(int argc, char **argv);

    private:
        std::string m_category;
        std::string m_testCaseID;
        std::string m_testDataPath;
        std::string m_configPath;
        std::string m_expOut;
};

#endif /*TEST_OPTION__H*/
