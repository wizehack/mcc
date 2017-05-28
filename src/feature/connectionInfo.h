#ifndef CONNECTOIN_INFO_H_
#define CONNECTOIN_INFO_H_

#include "mcHubType.h"

namespace mcHubd {
    class Connection {
        public:
            Connection(struct sockaddr_in sockAddr, int sockfd) :
                m_sockAddr(sockAddr), m_sockfd(sockfd), m_alive(false) {}
            ~Connection() {}
            void setAlive(bool alive) { m_alive = alive; }
            bool isAlive() { return m_alive; }
            void closeSocket() { close(m_sockfd); }
            int getSockfd() { return m_sockfd; }

        private:
            Connection();

        private:
            struct sockaddr_in m_sockAddr;
            int m_sockfd;
            bool m_alive;
    };

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
            std::vector<std::string> getAcceptedList() const;
            std::map<std::string, key_t> getAvailableList() const;
            std::map<std::string, pid_t> getConnectedClientKeyMap() const;

            /* socket connection */
            void openConnection(struct sockaddr_in sockAddr, int sockfd);
            std::map<int, std::shared_ptr<mcHubd::Connection>> getConnectionPool();
            void closeConnection(int sockfd);

        private:
            ConnectionInfo();

        private:
            std::vector<std::string> m_acceptedList;
            std::map<std::string, pid_t> m_connectedProcessMap;
            std::map<std::string, key_t> m_availableListMap;
            std::map<int, std::shared_ptr<mcHubd::Connection>> m_connPool;
            static std::atomic<ConnectionInfo*> _singleton;
            static std::mutex _mutex;
    };
}

#endif /*CONNECTOIN_INFO_H_*/
