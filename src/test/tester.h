#ifndef TESTER_H_
#define TESTER_H_

#include <string>
#include <map>
#include "testOption.h"

typedef bool (*FuncPtr)(void);

class Tester {
    public:
        Tester();
        virtual ~Tester();
        void setNext(Tester* tester);
        virtual bool request(TestOption* opt) = 0;

    protected:
        virtual void registerTestCase() = 0;
        bool exec(int tcId);
        void add(int tcId, FuncPtr func_cb);

    protected:
        Tester* m_next;
        FuncPtr m_testFunc_cb;
        std::map<int, FuncPtr> m_cbFuncMap;
};

#endif /*TESTER_H_*/
