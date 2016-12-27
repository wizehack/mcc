#ifndef TEST_STUB_H_
#define TEST_STUB_H_

#include <mutex>
#include <atomic>
#include <string>
#include <map>

namespace mcHubd {
    class TestStub {
        public:
            ~TestStub();
            static TestStub* getInstance();
            void addRespMsg(std::string msg);
            std::string getRespMsg(int id);

        private:
            TestStub();
            std::map<int, std::string> m_respMsgMap;
            int m_id;

        private:
            static std::atomic<TestStub*> _singleton;
            static std::mutex _mutex;
    };
}


#endif /*TEST_STUB_H_*/
