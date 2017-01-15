#ifndef TESTER_H_
#define TESTER_H_

#include <string>
#include <map>
#include "testOption.h"
#include "../mcHubType.h"

typedef bool (*FuncPtr)(void);

class TestSuite {
    public:
        TestSuite();
        virtual ~TestSuite();

        void setNext(TestSuite* testSuite);
        virtual bool request(TestOption* opt) = 0;

    protected:
        virtual void registerTestCase() = 0;
        bool exec(int tcId);
        void add(int tcId, FuncPtr func_cb);
        static bool _verifyResponseError(struct json_object* jobj, mcHubd::RESPCODE code, std::string message);
//        static bool _verifyResponseOk(struct json_object* jobj);

    private:
        TestSuite(const TestSuite&);
        TestSuite& operator=(const TestSuite& rhs);

    protected:
        TestSuite* m_next;
        FuncPtr m_testFunc_cb;
        std::map<int, FuncPtr> m_cbFuncMap;
};

#endif /*TESTER_H_*/
