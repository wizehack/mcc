#ifndef CONTRACT_H_
#define CONTRACT_H_

#include "mcHubType.h"
#include "messageHandler.h"

namespace mcHubd {
    class Contract {
        public:
            Contract(int contractID);
            Contract();
            ~Contract();

            void setProcessName(std::string name);
            void setClientKey(std::string name);
            void setProcessId(pid_t pid);
            void setChannel(key_t channel);
            void setContractReason(mcHubd::CONTRACTREASON reason);
            void setRespCode(mcHubd::RESPCODE code);

            std::string getProcessName();
            std::string getClientKey();
            pid_t getProcessId();
            key_t getChannel();
            int getContractID();
            mcHubd::CONTRACTREASON getContractReason();
            mcHubd::RESPCODE getRespCode();

        private:
            std::string m_processName;
            std::string m_clientKey;
            pid_t m_pid;
            key_t m_channel;
            mcHubd::RESPCODE m_respCode;
            int m_contractID;
            mcHubd::CONTRACTREASON m_reason;
    };
}

#endif /*CONTRACT_H_*/
