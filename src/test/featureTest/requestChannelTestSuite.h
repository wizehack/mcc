#ifndef REQUEST_CHANNEL_TEST_SUITE_H_
#define REQUEST_CHANNEL_TEST_SUITE_H_

#include "../testSuite.h"

class RequestChannelTestSuite : public TestSuite {
    public:
        RequestChannelTestSuite();
        ~RequestChannelTestSuite();
        bool request(TestOption* opt);

        static bool _testRequestReadyChannel();
        static bool _testRequestNotReadyChannel();
        static bool _verifyResponseOk(struct json_object* jobj, std::string key, std::string state, int channel);

    protected:
        void registerTestCase();

    private:
        static bool _setPrecondition();

    private:
        static std::string _testDataPath;
        static std::string _configPath;
};

#endif /* REQUEST_CHANNEL_TEST_SUITE_H_ */
