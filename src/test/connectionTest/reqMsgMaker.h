#ifndef REQ_MSG_MAKER_H_
#define REQ_MSG_MAKER_H_

#include "../../mcHubType.h"

class ReqMsgMaker {
    public:
        ReqMsgMaker();
        ~ReqMsgMaker();

        void setPid(int pid);
        void setProcessName(std::string psName);
        void setChannelList(std::list<std::string> keyList);

        std::string makeRegisterClientMsg();
        std::string makeRegisterChannelMsg(std::string key, int channel);
        std::string makeDeleteChannelMsg(std::string key, int channel);
        std::string makeDeleteClientMsg();
        std::map<std::string, int> getKeyChannelMap(std::string payload);

    private:
        int m_pid;
        std::string m_psName;
        std::list<std::string> m_keyList;
};
#endif /*REQ_MSG_MAKER_H_*/
