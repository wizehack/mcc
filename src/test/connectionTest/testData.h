#ifndef TEST_DATA_H_
#define TEST_DATA_H_

class TestData {
    public:
        TestData(std::string dataFile);
        ~TestData();
        std::list<std::string> getKeyList() const;
        std::string getProcessName() const;

    private:
        TestData();

    private:
        std::string m_processName;
        std::list<std::string> m_keyList;

};
#endif /*TEST_DATA_H_*/
