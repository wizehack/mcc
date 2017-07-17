#ifndef REGISTER_CLIENT_TEST_SUITE_H_
#define REGISTER_CLIENT_TEST_SUITE_H_

#include "../testSuite.h"

class RegisterClientTestSuite : public TestSuite {
    public:
        RegisterClientTestSuite();
        ~RegisterClientTestSuite();
        bool request(TestOption* opt);
        static bool _verifyResponseOk(struct json_object* jobj);

    protected:
        void registerTestCase();

    private:
        static bool _test();
        static bool _setPrecondition();

    private:
        static int _pid;
        static std::string _testDataPath;
        static std::string _psName;
        static std::list<std::string> _keyList;
};
#endif /*REGISTER_CLIENT_TEST_SUITE_H_*/
