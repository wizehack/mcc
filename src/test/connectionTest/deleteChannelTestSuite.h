#ifndef DELETE_CHANNEL_TEST_SUITE_H_
#define DELETE_CHANNEL_TEST_SUITE_H_

#include "../testSuite.h"

class DeleteChannelTestSuite : public TestSuite {
    public:
        DeleteChannelTestSuite();
        ~DeleteChannelTestSuite();
        bool request(TestOption* opt);

    protected:
        void registerTestCase();
        static bool _verifyResponseOk(struct json_object* jobj);

    private:
        static bool _test();
        static bool _setPrecondition();
        static bool _verifyResponseOk(struct json_object* jobj, std::string key, std::string state, int channel);

    private:
        static int _pid;
        static std::string _testDataPath;
        static std::string _psName;
        static std::list<std::string> _keyList;
};
#endif /*DELETE_CHANNEL_TEST_SUITE_H_*/
