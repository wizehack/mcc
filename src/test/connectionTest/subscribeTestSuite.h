#ifndef SUBSCRIBE_TEST_SUITE_H_
#define SUBSCRIBE_TEST_SUITE_H_

#include "../testSuite.h"

class SubscribeTestSuite : public TestSuite {
    public:
        SubscribeTestSuite();
        ~SubscribeTestSuite();
        bool request(TestOption* opt);
        static bool _verify(struct json_object* jobj);
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
#endif /*SUBSCRIBE_TEST_SUITE_H_*/
