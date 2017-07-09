#ifndef CONFIGURATOR_H_
#define CONFIGURATOR_H_

#include "mcHubType.h"
#include "confData.h"

namespace mcHubd {
    class Configurator {
        public:
            Configurator();
            ~Configurator();
            void initialize(std::map<std::string, std::string> optMap);
            bool isInitialized() const;
            std::string getAcceptedKeyPath() const;
            std::string getIPAddr() const;
            int getPort() const;

        private:
            Configurator(const mcHubd::Configurator&);
            void operator=(const Configurator&);
            bool parseConfFile(std::string confPath);

        private:
            ConfData* m_conf;
    };
}
#endif /*CONFIGURATOR_H_*/
