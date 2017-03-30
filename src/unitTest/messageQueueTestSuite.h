#ifndef MESSAGE_QUEUE_TEST_SUITE_H_
#define MESSAGE_QUEUE_TEST_SUITE_H_

#include "testSuite.h"
#include "../mcHubType.h"

class MessageQueueTestSuite : public TestSuite {
    public:
        MessageQueueTestSuite();
        ~MessageQueueTestSuite();
        bool request(TestOption* opt);
        static bool _testOKResponse();

    protected:
        void registerTestCase();

    private:
        static std::map<std::string, key_t> _getkeyChannelMap(std::string response);
        static bool _checkRequestType(std::string keyChannelResponse, mcHubd::MSGTYPE type);

    private:
        static std::string _testDataPath;
        static int _respIndex;

};

#endif /*MESSAGE_QUEUE_TEST_SUITE_H_*/
