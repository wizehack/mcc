#ifndef DELETE_CLIENT_TEST_SUITE_H_
#define DELETE_CLIENT_TEST_SUITE_H_

#include "../testSuite.h"

class DeleteClientTestSuite : public TestSuite {
    public:
        DeleteClientTestSuite();
        ~DeleteClientTestSuite();
        bool request(TestOption* opt);

    protected:
        void registerTestCase();
        static bool _verifyResponseOk(struct json_object* jobj);

    private:
        static bool _test();
        static bool _setPrecondition();

    private:
        static int _pid;
        static std::string _testDataPath;
        static std::string _psName;
        static std::list<std::string> _keyList;
};
#endif /*DELETE_CLIENT_TEST_SUITE_H_*/
