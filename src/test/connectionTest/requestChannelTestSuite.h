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
        static bool _verifyResponseOk(struct json_object* jobj);

    private:
        static bool _test();
        static bool _setPrecondition();

    private:
        static std::string _testDataPath;
        static std::string _psName;
        static std::list<std::string> _keyList;
};

#endif /* REQUEST_CHANNEL_TEST_SUITE_H_ */
