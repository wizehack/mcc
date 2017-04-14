#include "manager.h"
#include "connectionInfo.h"
#include "taskSet.h"

mcHubd::Manager::Manager(mcHubd::Mediator* mediator, std::string role):
    m_mediator(mediator),
    m_role(role),
    m_cInfo(mcHubd::ConnectionInfo::getInstance()){}
    mcHubd::Manager::~Manager(){}

std::string mcHubd::Manager::getRole() const
{
    return this->m_role;
}

void mcHubd::Manager::_setUpAcceptedList(std::string& acceptedKeyFilePath)
{
    struct json_object* jobj = NULL;
    struct json_object* listJobj = NULL;
    struct array_list* arrList = NULL;

    int arrSize = 0;
    int arrIndex = 0;

    std::string key;

    jobj = json_object_from_file(acceptedKeyFilePath.c_str());

    if( (jobj == NULL) || is_error(jobj) )
    {
        //error
        std::cout << "accepted key list file is NOT json" << std::endl;
        return;
    }

    if(!json_object_object_get_ex(jobj, "acceptedList", &listJobj))
    {
        //error
        std::cout << "acceptedList key is NOT found" << std::endl;
        json_object_put(jobj);
        return;
    }

    arrList = json_object_get_array(listJobj);

    if(arrList == NULL)
    {
        std::cout << "acceptedList is NULL" << std::endl;
        json_object_put(jobj);
        return;
    }

    arrSize = array_list_length(arrList);

    for(arrIndex = 0; arrIndex < arrSize; arrIndex++)
    {
        struct json_object* arrItemJobj = NULL;
        arrItemJobj = static_cast<json_object*>(array_list_get_idx(arrList, arrIndex));
        key.assign(json_object_get_string(arrItemJobj));
        mcHubd::ConnectionInfo::getInstance()->addAcceptedKey(key);
    }

    json_object_put(jobj);
}

void mcHubd::Manager::clearAcceptedList(std::string& acceptedKeyFilePath)
{
    // check the status of client in the availableListMap
    // remove clients in the availableListMap if thay are unAvailable.

    this->m_cInfo->clearAcceptedKeyList();
    this->m_cInfo->clearAvailableKeyList();
    this->m_cInfo->clearConnecteProcessMap();
    mcHubd::TaskSet::getInstance()->clearWaitingTask();
}

bool mcHubd::Manager::hasAcceptedList()
{
    std::vector<std::string> keyList = this->m_cInfo->getAcceptedList();
    if(keyList.empty())
        return false;
    return true;
}

bool mcHubd::Manager::isAccepted(std::string& key)
{
    std::vector<std::string> keyList = this->m_cInfo->getAcceptedList();

    if(keyList.empty())
        return false;

    std::vector<std::string>::iterator it;
    it = std::find(keyList.begin(), keyList.end(), key);

    if(it == keyList.end())
        return false;

    return true;
}

bool mcHubd::Manager::isAvailable(std::string& key)
{
    std::map<std::string, key_t> availKeyMap = this->m_cInfo->getAvailableList();

    if(availKeyMap.empty())
        return false;
    std::map<std::string, key_t>::iterator it;
    it = availKeyMap.find(key);

    if(it == availKeyMap.end())
        return false;

    return true;
}
