#ifndef REGISTER_CHANNEL_TEST_SUITE_H_
#define REGISTER_CHANNEL_TEST_SUITE_H_

#include "testSuite.h"

class RegisterChannelTestSuite : public TestSuite {
    public:
        RegisterChannelTestSuite();
        ~RegisterChannelTestSuite();
        bool request(TestOption* opt);

        static bool _testRegisterCreatedChannel();
        static bool _testRegisterNOTCreatedChannel();
        static bool _testRegisterDuplicatedChannel();

    protected:
        void registerTestCase();

    private:
        bool setPrecondition();

    private:
        static std::string _testDataPath;

};
#endif /*REGISTER_CHANNEL_TEST_SUITE_H_*/
