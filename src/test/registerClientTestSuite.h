#ifndef REGISTER_CLIENT_TEST_SUITE_H_
#define REGISTER_CLIENT_TEST_SUITE_H_

#include "testSuite.h"

class RegisterClientTestSuite : public TestSuite {
    public:
        RegisterClientTestSuite();
        ~RegisterClientTestSuite();
        bool request(TestOption* opt);

        static bool _testRegisterClientWithOneKey();
        static bool _testRegisterClientWithManyKey();
        static bool _testRegisterClientWithMAXKey();
        static bool _testRegisterClientWitUnlimitedKey();
        static bool _testRegisterClientWitUnacceptedKey();

    protected:
        void registerTestCase();

    private:
        static std::string _testDataPath;

};
#endif /*REGISTER_CLIENT_TEST_SUITE_H_*/
