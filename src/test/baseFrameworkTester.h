#ifndef BASE_FRAMEWORK_TESTER_H_
#define BASE_FRAMEWORK_TESTER_H_

#include "tester.h"

class BaseFrameworkTester : public Tester{
    public:
        BaseFrameworkTester();
        ~BaseFrameworkTester();
        bool request(TestOption* opt);

    protected:
        void registerTestCase();

    private:
        static std::string _testDataPath;
};

#endif /*BASE_FRAMEWORK_TESTER_H_*/
