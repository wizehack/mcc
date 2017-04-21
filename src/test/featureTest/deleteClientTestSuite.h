#ifndef DELETE_CLIENT_TEST_SUITE_H_
#define DELETE_CLIENT_TEST_SUITE_H_

#include "../testSuite.h"

class DeleteClientTestSuite : public TestSuite {
    public:
        DeleteClientTestSuite();
        ~DeleteClientTestSuite();
        bool request(TestOption* opt);
        static bool _testDeleteClient();
        static bool _testInvalidRequestMessage();
        static bool _testInternalError();
        static bool _testOKResponse();

    protected:
        void registerTestCase();
        static bool _verifyResponseOk(struct json_object* jobj);

    private:
        static bool _setPrecondition();
        static bool _parseProcessList(struct json_object* jobj);
        static bool _parseRegisteredKeyList(struct json_object* jobj);

    private:
        int m_testCaseID;
        static std::string _testDataPath;

};
#endif /*DELETE_CLIENT_TEST_SUITE_H_*/
