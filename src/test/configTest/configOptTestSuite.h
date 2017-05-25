#ifndef REQUEST_CHANNEL_TEST_SUITE_H_
#define REQUEST_CHANNEL_TEST_SUITE_H_

#include "../testSuite.h"

class ConfigOptionTestSuite : public TestSuite {
    public:
        ConfigOptionTestSuite();
        ~ConfigOptionTestSuite();
        bool request(TestOption* opt);

        static bool _testInitialized();
        static bool _testInitDefault();
        static bool _testNOTInitialized();

    protected:
        void registerTestCase();

    private:
        static std::map<std::string, std::string> _confOpt;
        static std::string _ip;
};

#endif /* REQUEST_CHANNEL_TEST_SUITE_H_ */
