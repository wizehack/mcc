#include <iostream>
#include "mcHubType.h"
#include "conf/configurator.h"
#include "configOptTestSuite.h"

std::map<std::string, std::string> ConfigOptionTestSuite::_confOpt;
std::string ConfigOptionTestSuite::_ip;

ConfigOptionTestSuite::ConfigOptionTestSuite() : TestSuite(){}
ConfigOptionTestSuite::~ConfigOptionTestSuite(){}

bool ConfigOptionTestSuite::request(TestOption* opt)
{
    if(opt == NULL)
        return false;

    if(opt->getCategory().compare("ConfigOption") == 0)
    {
        mcHubd::Configurator conf;
        std::string confFilePath = opt->getConfigPath();
        ConfigOptionTestSuite::_ip = opt->getExpectedOut();
        ConfigOptionTestSuite::_confOpt.insert(std::pair<std::string, std::string>("--config", confFilePath));

        this->registerTestCase();
        return this->exec(opt->getTestCaseID());
    }
    else if(this->m_next)
    {
        return this->m_next->request(opt);
    }

    return false;
}

void ConfigOptionTestSuite::registerTestCase()
{
    this->add(1, ConfigOptionTestSuite::_testInitialized);
    this->add(2, ConfigOptionTestSuite::_testInitDefault);
    this->add(3, ConfigOptionTestSuite::_testNOTInitialized);
}

bool ConfigOptionTestSuite::_testInitialized()
{
    mcHubd::Configurator conf;
    conf.initialize(ConfigOptionTestSuite::_confOpt);

    if(conf.isInitialized() == false)
        return false;

    std::string accpetedListFilePath = conf.getAcceptedKeyPath();
    std::string ipAddr = conf.getIPAddr();
    int port = conf.getPort();

    if(accpetedListFilePath.empty())
        return false;

    if(ipAddr.compare(ConfigOptionTestSuite::_ip) != 0)
        return false;

    if(port != 7070)
        return false;

    return true;
}

bool ConfigOptionTestSuite::_testInitDefault()
{
    mcHubd::Configurator conf;
    conf.initialize(ConfigOptionTestSuite::_confOpt);

    if(conf.isInitialized() == false)
        return false;

    std::string accpetedListFilePath = conf.getAcceptedKeyPath();
    std::string ipAddr = conf.getIPAddr();
    int port = conf.getPort();

    if(accpetedListFilePath.empty())
        return false;

    if(ipAddr.compare(ConfigOptionTestSuite::_ip) != 0)
        return false;

    if(port != 6000)
        return false;

    return true;
}

bool ConfigOptionTestSuite::_testNOTInitialized()
{
    mcHubd::Configurator conf;

    if(conf.isInitialized())
        return false;

    return true;
}
