#ifndef CONNECTOIN_INFO_H_
#define CONNECTOIN_INFO_H_

#include "mcHubType.h"

namespace mcHubd {
    class ConnectionInfo {
        public:
            ~ConnectionInfo();
            static ConnectionInfo* getInstance();
            void addAcceptedKey(std::string cKey);
            void addConnectedClientKeyMap(std::string cKey, pid_t pid);
            void addAvailableKey(std::string cKey, key_t ch);
            void clearAcceptedKeyList();
            void clearAvailableKeyList();
            void clearConnecteProcessMap();
            void removeAvailalbeKey(std::string& cKey);
            void deleteConnectedProcess(pid_t pid);
            std::vector<std::string> getConnectedClientKey(pid_t pid);
            std::vector<std::string> getAcceptedList();
            std::map<std::string, key_t> getAvailableList();
            std::map<std::string, pid_t> getConnectedClientKeyMap();

        private:
            ConnectionInfo();

        private:
            std::vector<std::string> m_acceptedList;
            std::map<std::string, pid_t> m_connectedProcessMap;
            std::map<std::string, key_t> m_availableListMap;
            static std::atomic<ConnectionInfo*> _singleton;
            static std::mutex _mutex;
    };
}

#endif /*CONNECTOIN_INFO_H_*/
