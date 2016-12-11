#ifndef DELETE_CHANNEL_TEST_SUITE_H_
#define DELETE_CHANNEL_TEST_SUITE_H_

#include "testSuite.h"

class DeleteChannelTestSuite : public TestSuite {
    public:
        DeleteChannelTestSuite();
        ~DeleteChannelTestSuite();
        bool request(TestOption* opt);

        static bool _testDeleteAvailableKey();
        static bool _testDeleteReadyKey();
        static bool _testDeleteEmptyChannel();
        static bool _testDeleteUnknownChannel();

    protected:
        void registerTestCase();

    private:
        bool setPrecondition();
        bool parseProcessList(struct json_object* jobj);
        bool parseConnectedKeyList(struct json_object* jobj);
        bool parseRegisteredKeyList(struct json_object* jobj);

    private:
        int m_testCaseID;
        static std::string _testDataPath;

};
#endif /*DELETE_CHANNEL_TEST_SUITE_H_*/
