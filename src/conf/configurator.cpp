#include "configurator.h"

mcHubd::Configurator::Configurator():
    m_conf(mcHubd::ConfData::getInstance()){}
mcHubd::Configurator::~Configurator(){}

void mcHubd::Configurator::initialize(std::map<std::string, std::string> optMap)
{
    bool bInitialized = this->m_conf->isInitialized();
    std::string confFilePath;
    std::map<std::string, std::string>::iterator itor;

    if(bInitialized)
        return;

    if(optMap.empty())
        return;

    itor = optMap.find("--config");

    if(itor != optMap.end())
        confFilePath.assign(itor->second);

    if(this->parseConfFile(confFilePath) == false)
    {
        std::cout << "Configurator Error! Path: " << confFilePath << std::endl;
        exit(1);
    }

    m_conf->initialize();
}

bool mcHubd::Configurator::isInitialized() const
{
    return m_conf->isInitialized();
}

std::string mcHubd::Configurator::getAcceptedKeyPath() const
{
    return m_conf->getAcceptedKeyPath();
}

std::string mcHubd::Configurator::getIPAddr() const
{
    return m_conf->getIPAddr();
}

int mcHubd::Configurator::getPort() const
{
    return m_conf->getPort();
}

bool mcHubd::Configurator::parseConfFile(std::string confPath)
{
    struct json_object* jobj = NULL;
    struct json_object* acceptedListPathJobj = NULL;
    struct json_object* ipJobj = NULL;
    struct json_object* portJobj = NULL;

    std::string acceptedListPath;
    std::string ipAddr("127.0.0.1");
    int port = 6000;
    bool isVaild = true;

    jobj = json_object_from_file(confPath.c_str());

    if( (jobj == NULL) || is_error(jobj) )
        return false;

    if(json_object_object_get_ex(jobj, "acceptedList", &acceptedListPathJobj))
    {
        if(json_object_is_type(acceptedListPathJobj, json_type_string))
            acceptedListPath.assign(json_object_get_string(acceptedListPathJobj));
        else
            isVaild = false;
    }
    else
        isVaild = false;

    if(json_object_object_get_ex(jobj, "ip", &ipJobj))
    {
        if(json_object_is_type(ipJobj, json_type_string))
            ipAddr.assign(json_object_get_string(ipJobj));
        else
            isVaild = false;
    }

    if(json_object_object_get_ex(jobj, "port", &portJobj))
    {
        if(json_object_is_type(portJobj, json_type_int))
            port = json_object_get_int(portJobj);
        else
            isVaild = false;
    }

    this->m_conf->setAcceptedKeyPath(acceptedListPath);
    this->m_conf->setIPAddr(ipAddr);
    this->m_conf->setPort(port);

    json_object_put(jobj);
    return isVaild;
}
