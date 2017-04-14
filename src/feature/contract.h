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
            void setRespCode(mcHubd::RESPCODE code);
            void setChannelStatus(mcHubd::STATUS status);

            std::string getProcessName() const;
            std::string getClientKey() const;

            pid_t getProcessId() const;
            key_t getChannel() const;
            int getContractID() const;
            mcHubd::RESPCODE getRespCode() const;
            mcHubd::STATUS getChannelStatus() const;

        private:
            std::string m_processName;
            std::string m_clientKey;
            pid_t m_pid;
            key_t m_channel;
            mcHubd::RESPCODE m_respCode;
            int m_contractID;
            mcHubd::CONTRACTREASON m_reason;
            mcHubd::STATUS m_status;
    };
}

#endif /*CONTRACT_H_*/
