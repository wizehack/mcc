#ifndef REQUEST_CHANNEL_TEST_SUITE_H_
#define REQUEST_CHANNEL_TEST_SUITE_H_

#include "../testSuite.h"

class RequestChannelTestSuite : public TestSuite {
    public:
        RequestChannelTestSuite();
        ~RequestChannelTestSuite();
        bool request(TestOption* opt);

    protected:
        void registerTestCase();
        static bool _verifyResponseOk(struct json_object* jobj, std::string key, std::string state);

    private:
        static bool _test();
        static bool _setPrecondition();

    private:
        static std::string _testDataPath;
        static int _pid;
        static int _tarPid;
        static std::string _psName;
        static std::string _targetPsName;
        static std::list<std::string> _keyList;
        static std::list<std::string> _targetKeyList;
};

#endif /* REQUEST_CHANNEL_TEST_SUITE_H_ */
