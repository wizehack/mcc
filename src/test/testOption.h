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
        int getTestCaseID();

    private:
        TestOption();
        void parse(int argc, char **argv);

    private:
        std::string m_category;
        std::string m_testCaseID;
        std::string m_testDataPath;
        std::string m_configPath;
};

#endif /*TEST_OPTION__H*/
