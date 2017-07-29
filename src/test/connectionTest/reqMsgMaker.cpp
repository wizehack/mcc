#include "reqMsgMaker.h"

ReqMsgMaker::ReqMsgMaker():
    m_pid(0),
    m_psName(),
    m_keyList(){}

ReqMsgMaker::~ReqMsgMaker(){}

void ReqMsgMaker::setPid(int pid)
{
    m_pid = pid;
}

void ReqMsgMaker::setProcessName(std::string psName)
{
    m_psName = psName;
}

void ReqMsgMaker::setChannelList(std::list<std::string> keyList)
{
    m_keyList = keyList;
}

std::string ReqMsgMaker::makeRegisterClientMsg()
{
    std::string reqMsg;
    struct json_object* jobj = json_object_new_object();
    struct json_object* typeJobj = json_object_new_string("RegisterClient");
    struct json_object* msgJobj = json_object_new_object();
    struct json_object* pidJobj = json_object_new_int(this->m_pid);
    struct json_object* psNameJobj = json_object_new_string(this->m_psName.c_str());
    struct json_object* keyList = json_object_new_array();

    std::list<std::string>::iterator it;

    for(it = this->m_keyList.begin(); it != this->m_keyList.end(); ++it)
    {
        std::string key = (*it);
        struct json_object* keyJobj = json_object_new_string(key.c_str());
        json_object_array_add(keyList, keyJobj);
    }

    json_object_object_add(msgJobj, "pid", pidJobj);
    json_object_object_add(msgJobj, "psName", psNameJobj);
    json_object_object_add(msgJobj, "keyList", keyList);

    json_object_object_add(jobj, "msgType", typeJobj);
    json_object_object_add(jobj, "message", msgJobj);

    reqMsg.assign(json_object_get_string(jobj));

    json_object_put(jobj);

    return reqMsg;
}

std::string ReqMsgMaker::makeRegisterClientMsgWithSubscribe()
{
    std::string reqMsg;
    struct json_object* jobj = json_object_new_object();
    struct json_object* typeJobj = json_object_new_string("RegisterClient");
    struct json_object* msgJobj = json_object_new_object();
    struct json_object* subscribeJobj = json_object_new_boolean(true);
    struct json_object* pidJobj = json_object_new_int(this->m_pid);
    struct json_object* psNameJobj = json_object_new_string(this->m_psName.c_str());
    struct json_object* keyList = json_object_new_array();

    std::list<std::string>::iterator it;

    for(it = this->m_keyList.begin(); it != this->m_keyList.end(); ++it)
    {
        std::string key = (*it);
        struct json_object* keyJobj = json_object_new_string(key.c_str());
        json_object_array_add(keyList, keyJobj);
    }

    json_object_object_add(msgJobj, "pid", pidJobj);
    json_object_object_add(msgJobj, "subscribe", subscribeJobj);
    json_object_object_add(msgJobj, "psName", psNameJobj);
    json_object_object_add(msgJobj, "keyList", keyList);

    json_object_object_add(jobj, "msgType", typeJobj);
    json_object_object_add(jobj, "message", msgJobj);

    reqMsg.assign(json_object_get_string(jobj));

    json_object_put(jobj);

    return reqMsg;
}

std::string ReqMsgMaker::makeRegisterChannelMsg(std::string key, int channel)
{
    std::string reqMsg;
    struct json_object* jobj = json_object_new_object();
    struct json_object* typeJobj = json_object_new_string("RegisterChannel");
    struct json_object* msgJobj = json_object_new_object();
    struct json_object* keyJobj = json_object_new_string(key.c_str());
    struct json_object* channelJobj = json_object_new_int(channel);

    json_object_object_add(msgJobj, "key", keyJobj);
    json_object_object_add(msgJobj, "channel", channelJobj);

    json_object_object_add(jobj, "msgType", typeJobj);
    json_object_object_add(jobj, "message", msgJobj);

    reqMsg.assign(json_object_get_string(jobj));

    json_object_put(jobj);

    return reqMsg;
}

std::string ReqMsgMaker::makeDeleteChannelMsg(std::string key, int channel)
{
    std::string reqMsg;
    struct json_object* jobj = json_object_new_object();
    struct json_object* typeJobj = json_object_new_string("DeleteChannel");
    struct json_object* msgJobj = json_object_new_object();
    struct json_object* keyJobj = json_object_new_string(key.c_str());
    struct json_object* channelJobj = json_object_new_int(channel);

    json_object_object_add(msgJobj, "key", keyJobj);
    json_object_object_add(msgJobj, "channel", channelJobj);

    json_object_object_add(jobj, "msgType", typeJobj);
    json_object_object_add(jobj, "message", msgJobj);

    reqMsg.assign(json_object_get_string(jobj));

    json_object_put(jobj);

    return reqMsg;
}

std::string ReqMsgMaker::makeDeleteClientMsg()
{
    std::string reqMsg;
    struct json_object* jobj = json_object_new_object();
    struct json_object* typeJobj = json_object_new_string("DeleteClient");
    struct json_object* msgJobj = json_object_new_object();
    struct json_object* pidJobj = json_object_new_int(this->m_pid);
    struct json_object* psNameJobj = json_object_new_string(this->m_psName.c_str());

    json_object_object_add(msgJobj, "pid", pidJobj);
    json_object_object_add(msgJobj, "psName", psNameJobj);

    json_object_object_add(jobj, "msgType", typeJobj);
    json_object_object_add(jobj, "message", msgJobj);

    reqMsg.assign(json_object_get_string(jobj));

    json_object_put(jobj);

    return reqMsg;
}

std::string ReqMsgMaker::makeRequestChannelMsg(std::string destKey)
{
    std::string reqMsg;
    struct json_object* jobj = json_object_new_object();
    struct json_object* typeJobj = json_object_new_string("GetChannel");
    struct json_object* msgJobj = json_object_new_object();
    struct json_object* keyJobj = json_object_new_string(destKey.c_str());

    json_object_object_add(msgJobj, "key", keyJobj);
    json_object_object_add(jobj, "msgType", typeJobj);
    json_object_object_add(jobj, "message", msgJobj);

    reqMsg.assign(json_object_get_string(jobj));

    json_object_put(jobj);

    return reqMsg;
}

std::map<std::string, int> ReqMsgMaker::getKeyChannelMap(std::string payload)
{
    std::map<std::string, int> map;
    struct json_object* jobj = json_tokener_parse(payload.c_str());
    struct json_object* messageJobj = NULL;
    struct array_list* keyChannelList = NULL;

    if((jobj == NULL) || is_error(jobj))
    {
        return map;
    }

    if(!json_object_object_get_ex(jobj, "message", &messageJobj))
    {
        json_object_put(jobj);
        return map;
    }

    keyChannelList = json_object_get_array(messageJobj);

    if(keyChannelList)
    {
        int arrSize = array_list_length(keyChannelList);
        int arrIndex = 0;

        for(arrIndex = 0; arrIndex < arrSize; arrIndex++)
        {
            struct json_object* keyChannelJobj = NULL;
            struct json_object* keyJobj = NULL;
            struct json_object* channelJobj = NULL;

            keyChannelJobj = static_cast<json_object*>(array_list_get_idx(keyChannelList, arrIndex));

            if(!json_object_object_get_ex(keyChannelJobj, "key", &keyJobj))
            {
                json_object_put(jobj);
                map.clear();
                return map;
            }

            if(!json_object_object_get_ex(keyChannelJobj, "channel", &channelJobj))
            {
                json_object_put(jobj);
                map.clear();
                return map;
            }

            if(keyJobj && channelJobj)
            {
                std::string key(json_object_get_string(keyJobj));
                int channel = json_object_get_int(channelJobj);
                map.insert(std::pair<std::string, int>(key, channel));
            }
            else
            {
                map.clear();
                return map;
            }
        }
    }

    return map;
}
