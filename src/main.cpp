#include "connection/connectionManager.h"
#include "conf/configurator.h"
#include "feature/manager.h"

GMainLoop *mainLoop;

int main(int argc, char **argv)
{
    mainLoop = g_main_loop_new (NULL, TRUE);

    mcHubd::Configurator conf;
    std::string configPath;
    int i = 1;

    if(argc < 2)
    {
        std::cout << "argc : " << argc << std::endl;
        return -1;
    }

    if (strcmp(argv[i], "--config") == 0)
    {
        std::map<std::string, std::string> confOpt;
        std::string acceptedKeyFilePath;

        configPath.assign(argv[i+1]);
        confOpt.insert(std::pair<std::string, std::string>("--config", configPath));
        conf.initialize(confOpt);
        acceptedKeyFilePath = conf.getAcceptedKeyPath();
        mcHubd::Manager::_setUpAcceptedList(acceptedKeyFilePath);
    }

    std::cout << "####################" << std::endl;
    mcHubd::ConnectionManager::start();

    g_main_loop_run (mainLoop);
    return 0;
}
