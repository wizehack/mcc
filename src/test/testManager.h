#ifndef TEST_MANAGER_H_
#define TEST_MANAGER_H_

class TestOption;
class TestManager {
    public:
        TestManager();
        ~TestManager();
        bool execute(TestOption* opt);
};

#endif /* TEST_MANAGER_H_ */
