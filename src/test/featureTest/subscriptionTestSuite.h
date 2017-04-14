#ifndef SUBSCRIB_TEST_SUITE_H_
#define SUBSCRIB_TEST_sUITE_H_

#include "../testSuite.h"

class SubscriptionTestSuite : public TestSuite {

    public:
        SubscriptionTestSuite();
        ~SubscriptionTestSuite();
        bool request(TestOption* opt);

        static bool _testChannelOpen();
        static bool _testChannelReady();
        static bool _testChannelCloseByDeleteChannel();
        static bool _testChannelCloseByDeleteClient();

        static bool _verifyResponse(struct json_object* jobj, std::string key, int channel, std::string state);


    protected:
        void registerTestCase();

    private:
        static bool _setPrecondition();
        static bool _setUpAcceptedList();
        static bool _setUpRegisteredChannel();
        static bool _setUpCreatedChannel();

    private:
        static std::string _testDataPath;

};
#endif /* SUBSCRIB_TEST_sUITE_H_ */
