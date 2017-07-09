#include "../../mcHubType.h"
#include "testData.h"

TestData::TestData(std::string dataFile):
    m_processName(),
    m_keyList()
{
    if(dataFile.empty() == false)
    {
        struct json_object* jobj = json_object_from_file(dataFile.c_str());

        if(jobj && !is_error(jobj))
        {
            struct json_object* psNameJobj = NULL;
            struct json_object* keyListJobj = NULL;

            if(json_object_object_get_ex(jobj, "name", &psNameJobj))
            {
                this->m_processName.assign(json_object_get_string(psNameJobj));
            }

            if(json_object_object_get_ex(jobj, "keys", &keyListJobj))
            {
                struct array_list* keyListArr = json_object_get_array(keyListJobj);
                int arrSize = array_list_length(keyListArr);
                int i;
                for(i=0; i<arrSize; i++)
                {
                    struct json_object* itemJobj = static_cast<json_object*>(array_list_get_idx(keyListArr, i));
                    std::string key(json_object_get_string(itemJobj));
                    this->m_keyList.push_back(key);
                }
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

TestData::~TestData(){}

std::list<std::string> TestData::getKeyList() const
{
    return this->m_keyList;
}

std::string TestData::getProcessName() const
{
    return this->m_processName;
}
