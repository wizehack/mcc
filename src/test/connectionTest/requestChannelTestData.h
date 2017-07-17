#ifndef REQUEST_CHANNEL_TEST_DATA_H_
#define REQUEST_CHANNEL_TEST_DATA_H_

class RequestChannelTestData {
    public:
        RequestChannelTestData(std::string dataFile);
        ~RequestChannelTestData();

        std::list<std::string> getKeyList() const;
        std::string getProcessName() const;
        int getPID() const;

        std::list<std::string> getTargetKeyList() const;
        std::string getTargetProcessName() const;
        int getTargetPID() const;

    private:
        RequestChannelTestData();
        std::string extractProcessName(struct json_object* jobj);
        std::list<std::string> extractKeyList(struct json_object* jobj);
        int extractPID(struct json_object* jobj);

    private:
        std::string m_processName;
        std::string m_targetProcessName;
        std::list<std::string> m_keyList;
        std::list<std::string> m_targetKeyList;
        int m_targetPID;
        int m_PID;
};

#endif /*REQUEST_CHANNEL_TEST_DATA_H_*/
