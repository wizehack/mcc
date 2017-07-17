#ifndef UNDEFINED_TYPE_TEST_SUITE_H
#define UNDEFINED_TYPE_TEST_SUITE_H

#include "../testSuite.h"

class UndefinedTypeTestSuite : public TestSuite {
    public:
        UndefinedTypeTestSuite();
        ~UndefinedTypeTestSuite();
        bool request(TestOption* opt);
        static bool _test();

    protected:
        void registerTestCase();
        static bool _verifyResponse(struct json_object* jobj);
};

#endif /*UNDEFINED_TYPE_TEST_SUITE_H*/
