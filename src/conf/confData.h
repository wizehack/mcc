#ifndef CONF_DATA_H_
#define CONF_DATA_H_

#include "mcHubType.h"

namespace mcHubd {
    class ConfData {
        public:
            ~ConfData();
            static ConfData* getInstance();

            void initialize();
            void setAcceptedKeyPath(std::string path);
            void setIPAddr(std::string ipAddr);
            void setPort(int port);

            bool isInitialized();
            std::string getAcceptedKeyPath();
            std::string getIPAddr();
            int getPort();

        private:
            ConfData();

        private:
            bool m_isInitialized;
            std::string m_acceptedKeyPath;
            std::string m_ipAddr;
            int m_port;

            static std::atomic<ConfData*> _singleton;
            static std::mutex _mutex;
    };
}

#endif /*CONF_DATA_H_*/
