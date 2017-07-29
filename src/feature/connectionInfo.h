#ifndef CONNECTOIN_INFO_H_
#define CONNECTOIN_INFO_H_

#include "mcHubType.h"

namespace mcHubd {
    class Socket;
    class ConnectionInfo {
        public:
            ~ConnectionInfo();
            static ConnectionInfo* getInstance();
            void registerServer(mcHubd::Socket* server);
            mcHubd::Socket* getServer();
            void addAcceptedKey(std::string cKey);
            void addProcess(std::string psName, pid_t pid);
            void addConnectedClientKeyMap(std::string cKey, pid_t pid);
            void addAvailableKey(std::string cKey, key_t ch);
            void clearAcceptedKeyList();
            void clearAvailableKeyList();
            void clearConnecteProcessMap();
            void removeAvailalbeKey(std::string& cKey);
            bool deleteConnectedProcess(pid_t pid, std::string psName);
            std::vector<std::string> getConnectedClientKey(pid_t pid);
            std::vector<std::string> getAcceptedList() const;
            std::map<std::string, key_t> getAvailableList() const;
            std::map<std::string, pid_t> getConnectedClientKeyMap() const;
            pid_t getPID(std::string psName);

            /* socket connection */
            void saveConnInfo(std::string psName, struct sockaddr_in sockAddr);
            std::map<std::string, struct sockaddr_in> getUDPConnPool() const;
            void deleteConnInfo(std::string psName);

        private:
            ConnectionInfo();
            ConnectionInfo(const ConnectionInfo&);
            void operator=(const ConnectionInfo&);
            void deleteConnectedClientKey(pid_t pid);

        private:
            std::vector<std::string> m_acceptedList;
            std::map<std::string, pid_t> m_connectedProcessMap; //psName, pid
            std::map<std::string, pid_t> m_connectedClientKeyMap; //key, pid
            std::map<std::string, key_t> m_availableListMap;
            std::map<std::string, struct sockaddr_in> m_connPool;
            mcHubd::Socket* m_server;
            static std::atomic<ConnectionInfo*> _singleton;
            static std::mutex _mutex;
    };
}

#endif /*CONNECTOIN_INFO_H_*/
