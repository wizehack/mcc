#ifndef DELETE_CLIENT_TEST_SUITE_H_
#define DELETE_CLIENT_TEST_SUITE_H_

#include "testSuite.h"

class DeleteClientTestSuite : public TestSuite {
    public:
        DeleteClientTestSuite();
        ~DeleteClientTestSuite();
        bool request(TestOption* opt);
        static bool _testDeleteClient();

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
#endif /*DELETE_CLIENT_TEST_SUITE_H_*/
