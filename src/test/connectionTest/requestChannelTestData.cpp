#include "../../mcHubType.h"
#include "requestChannelTestData.h"

RequestChannelTestData::RequestChannelTestData(std::string dataFile):
    m_processName(),
    m_targetProcessName(),
    m_keyList(),
    m_targetKeyList()
{
    if(dataFile.empty() == false)
    {
        struct json_object* jobj = json_object_from_file(dataFile.c_str());

        if(jobj && !is_error(jobj))
        {
            struct json_object* observableJobj = NULL;
            struct json_object* observerJobj = NULL;

            if(json_object_object_get_ex(jobj, "observable", &observableJobj))
            {
                this->m_targetProcessName = this->extractProcessName(observableJobj);
                this->m_targetKeyList = this->extractKeyList(observableJobj);
            }

            if(json_object_object_get_ex(jobj, "observer", &observerJobj))
            {
                this->m_processName = this->extractProcessName(observerJobj);
                this->m_keyList = this->extractKeyList(observerJobj);
            }

            json_object_put(jobj);
        }
        else
        {
            std::cout << dataFile << " is NOT json" << std::endl;
        }
    }
    else
    {
        std::cout << dataFile << " is NOT found" << std::endl;
    }
}

RequestChannelTestData::~RequestChannelTestData(){}

std::string RequestChannelTestData::extractProcessName(struct json_object* jobj)
{
    std::string psName;

    if(jobj && !is_error(jobj))
    {
        struct json_object* psNameJobj = NULL;

        if(json_object_object_get_ex(jobj, "name", &psNameJobj))
        {
            if(psNameJobj)
                psName.assign(json_object_get_string(psNameJobj));
        }
    }

    return psName;
}

std::list<std::string> RequestChannelTestData::extractKeyList(struct json_object* jobj)
{
    std::list<std::string> keyList;

    if(jobj && !is_error(jobj))
    {
        struct json_object* keyListJobj = NULL;

        if(json_object_object_get_ex(jobj, "keys", &keyListJobj))
        {
            if(keyListJobj)
            {
                struct array_list* keyListArr = json_object_get_array(keyListJobj);
                int arrSize = array_list_length(keyListArr);
                int i;

                for(i=0; i<arrSize; i++)
                {
                    struct json_object* itemJobj = static_cast<json_object*>(array_list_get_idx(keyListArr, i));

                    if(itemJobj)
                    {
                        std::string key(json_object_get_string(itemJobj));
                        this->m_keyList.push_back(key);
                    }
                }
            }
        }
    }

    return keyList;
}

std::list<std::string> RequestChannelTestData::getKeyList() const
{
    return this->m_keyList;
}

std::string RequestChannelTestData::getProcessName() const
{
    return this->m_processName;
}

std::list<std::string> RequestChannelTestData::getTargetKeyList() const
{
    return this->m_targetKeyList;
}

std::string RequestChannelTestData::getTargetProcessName() const
{
    return this->m_targetProcessName;
}
