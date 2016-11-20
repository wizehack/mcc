#include "registerClientHandler.h"
#include "clientManager.h"
#include "channelStatusMediator.h"
#include "channelManager.h"

mcHubd::RegisterClientHandler::RegisterClientHandler():
    m_pid(-1),
    m_processName(),
    MAX_NUMBER_OF_CLIENT_KEY(10){}
mcHubd::RegisterClientHandler::~RegisterClientHandler(){}

void mcHubd::RegisterClientHandler::request(std::shared_ptr<mcHubd::Message> msg)
{
    if(msg->getType() == REQ_REG_CLIENT)
    {
        std::string respMsg;
        mcHubd::RESPCODE code;
        mcHubd::Mediator* mediator = NULL;
        std::shared_ptr<mcHubd::ClientManager> clientMgr;
        std::shared_ptr<mcHubd::ChannelManager> channelMgr;

        // parse() assigns value to member fields; m_pid, m_processNameand m_cKeyList.
        if(this->parse(msg->getBody()) == false)
        {
            std::string emptyMsg;
            code = MCHUBD_INVALID_MSG;
            this->responseError(code, emptyMsg);
            return;
        }

        if(this->m_cKeyList.size() > MAX_NUMBER_OF_CLIENT_KEY)
        {
            std::string msg;
            msg = "DO NOT EXCEED " + std::to_string(MAX_NUMBER_OF_CLIENT_KEY);
            code = MCHUBD_EXCEEDED_MAXIMUM_CLIENT_KEY;
            this->responseError(code, msg);
            return;
        }

        mediator = new mcHubd::ChannelStatusMediator();

        if(mediator)
        {
            clientMgr = std::make_shared<mcHubd::ClientManager>(mediator);
            channelMgr = std::make_shared<mcHubd::ChannelManager>(mediator);

            mediator->appendManager(clientMgr);
            mediator->appendManager(channelMgr);

            respMsg = this->makeNewChannelList(mediator);

            if(respMsg.empty()) //error response was sent
            {
                mcHubd::Contract* contract = new mcHubd::Contract();

                if(contract)
                {
                    contract->setProcessName(this->m_processName);
                    contract->setProcessId(this->m_pid);
                    mediator->deleteClient(&contract);
                    delete contract;
                }
            }
            else
            {
                //send response message to client
                this->responseOK(respMsg);
            }

            delete mediator;
        }
    }
    else
    {
        if(this->m_next)
            this->m_next->request(msg);
    }
}

bool mcHubd::RegisterClientHandler::createChannelJobj(struct json_object** jobj, std::string cKey, key_t channel)
{
    struct json_object* keyJobj = NULL;
    struct json_object* channelJobj = NULL;

    if((*jobj) == NULL)
        return false;

    if(channel <= 0)
        return false;

    keyJobj = json_object_new_string(cKey.c_str());
    channelJobj = json_object_new_int(static_cast<int>(channel));

    json_object_object_add((*jobj), "key", keyJobj);
    json_object_object_add((*jobj), "channel", channelJobj);

    return true;
}

bool mcHubd::RegisterClientHandler::parse(std::string payload)
{
    struct json_object* jobj = NULL;
    struct json_object* processIdJobj = NULL;
    struct json_object* processNameJobj = NULL;
    struct json_object* keyListJobj = NULL;
    struct json_object* keyJobj = NULL;
    struct array_list* keyList = NULL;

    int arrSize = 0;
    int arrIndex = 0;

    //initialize member variables
    this->m_pid = -1;
    this->m_processName.clear();
    this->m_cKeyList.clear();

    if(payload.empty())
        return false;

    jobj = json_tokener_parse(payload.c_str());

    if((jobj == NULL) || is_error(jobj))
        return false;

    if(!json_object_object_get_ex(jobj, "psName", &processNameJobj))
    {
        json_object_put(jobj);
        return false;
    }

    this->m_processName.assign(json_object_get_string(processNameJobj));

    if(!json_object_object_get_ex(jobj, "pid", &processIdJobj))
    {
        json_object_put(jobj);
        return false;
    }

    this->m_pid = static_cast<pid_t>(json_object_get_int(processIdJobj));

    if(!json_object_object_get_ex(jobj, "keyList", &keyListJobj))
    {
        json_object_put(jobj);
        return false;
    }

    keyList = json_object_get_array(keyListJobj);

    if(keyList == NULL)
    {
        json_object_put(jobj);
        return false;
    }

    arrSize = array_list_length(keyList);

    for(arrIndex = 0; arrIndex < arrSize; arrIndex++)
    {
        keyJobj = static_cast<json_object*>(array_list_get_idx(keyList, arrIndex));
        std::string clientKey(json_object_get_string(keyJobj));
        this->m_cKeyList.push_back(clientKey);
    }

    json_object_put(jobj);
    return true;
}

std::string mcHubd::RegisterClientHandler::makeNewChannelList(mcHubd::Mediator* mediator)
{
    int sequenceNumber = 0;
    std::list<std::string>::iterator itor;
    std::string respMsg("[");

    if(mediator == NULL)
    {
        respMsg.clear();
        return respMsg;
    }

    // Each client sends its key list to create message channel
    // In this loop, new channel is created from the client key list
    for(itor = this->m_cKeyList.begin(); itor != this->m_cKeyList.end(); itor++)
    {
        mcHubd::RESPCODE code;
        struct json_object* jobj = NULL;
        std::string keyChannelJson;
        mcHubd::Contract* contract = NULL;

        contract = new mcHubd::Contract(sequenceNumber);
        sequenceNumber++;

        if(contract)
        {
            contract->setProcessName(this->m_processName);
            contract->setProcessId(this->m_pid);
            contract->setClientKey((*itor));
        }

        mediator->getNewChannel(&contract);

        if(contract == NULL)
        {
            code = MCHUBD_INTERNAL_ERROR;
            this->responseError(code, (*itor));
            respMsg.clear();
            return respMsg;
        }

        if(contract->getRespCode() == MCHUBD_OK)
        {
            jobj = json_object_new_object();

            if(this->createChannelJobj(&jobj, (*itor), contract->getChannel()) == false)
            {
                code = MCHUBD_CREATE_CHANNEL_ERROR;
                this->responseError(code, (*itor));
                json_object_put(jobj);
                respMsg.clear();
                return respMsg;
            }

            keyChannelJson.assign(json_object_get_string(jobj));

            //make json array [ json1, json2, ...]
            if(keyChannelJson.empty() == false)
            {
                itor++; //check if current key is last element or not
                if((itor) != this->m_cKeyList.end())
                    respMsg = respMsg + keyChannelJson.c_str() + ", ";
                else
                    respMsg = respMsg + keyChannelJson.c_str() + "]";
                itor--;
            }

            json_object_put(jobj);
        }
        else
        {
            this->responseError(contract->getRespCode(), (*itor));
            respMsg.clear();
            return respMsg;
        }
    }

    return respMsg;
}
