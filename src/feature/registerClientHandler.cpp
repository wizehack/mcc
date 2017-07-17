#include "registerClientHandler.h"
#include "clientManager.h"
#include "channelStatusMediator.h"
#include "channelManager.h"
#include "messageTransportService.h"

mcHubd::RegisterClientHandler::RegisterClientHandler():
    m_pid(-1),
    m_processName(),
    MAX_NUMBER_OF_CLIENT_KEY(10),
    m_cKeyList(),
    m_msg(NULL){}
mcHubd::RegisterClientHandler::~RegisterClientHandler(){}

bool mcHubd::RegisterClientHandler::request(mcHubd::Message* msg)
{
    if(msg->getType() == REQ_REG_CLIENT)
    {
        bool ret = false;
        mcHubd::RESPCODE code;
        mcHubd::Mediator* mediator = NULL;
        std::shared_ptr<mcHubd::ClientManager> clientMgr;
        std::shared_ptr<mcHubd::ChannelManager> channelMgr;

        this->m_msg = msg;

        // parse() assigns value to member fields; m_pid, m_processNameand m_cKeyList.
        if((this->parse(msg->getBody()) == false))
        {
            std::string emptyMsg;
            code = MCHUBD_INVALID_MSG;
            mcHubd::MessageHandler::_responseError(code, emptyMsg, this->m_msg);
            return false;
        }

        if(this->m_cKeyList.size() > MAX_NUMBER_OF_CLIENT_KEY)
        {
            std::string msg;
            msg = "DO NOT EXCEED " + std::to_string(MAX_NUMBER_OF_CLIENT_KEY);
            code = MCHUBD_EXCEEDED_MAXIMUM_CLIENT_KEY;
            mcHubd::MessageHandler::_responseError(code, msg, this->m_msg);
            return false;
        }

        mediator = new mcHubd::ChannelStatusMediator();

        if(mediator)
        {
            std::map<std::string, key_t> keyChannelMap;
            clientMgr = std::make_shared<mcHubd::ClientManager>(mediator);
            channelMgr = std::make_shared<mcHubd::ChannelManager>(mediator);

            mediator->appendManager(clientMgr);
            mediator->appendManager(channelMgr);

            keyChannelMap = this->makeNewChannelList(mediator);

            if(keyChannelMap.empty()) //error response was sent
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
                struct sockaddr_in clientAddr = this->m_msg->getSockAddr();
                mcHubd::RegisterClientHandler::_responseOK(keyChannelMap, this->m_msg);
                mcHubd::ConnectionInfo::getInstance()->saveConnInfo(this->m_processName, clientAddr);
                ret = true;
            }

            delete mediator;
        }

        return ret;
    }
    else
    {
        if(this->m_next)
            return this->m_next->request(msg);
        else
            return false;
    }
}

bool mcHubd::RegisterClientHandler::parse(std::string payload)
{
    struct json_object* jobj = NULL;
    struct json_object* processIdJobj = NULL;
    struct json_object* processNameJobj = NULL;
    struct json_object* keyListJobj = NULL;
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

    if(processNameJobj)
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

    if(arrSize < 1)
    {
        json_object_put(jobj);
        return false;
    }

    for(arrIndex = 0; arrIndex < arrSize; arrIndex++)
    {
        struct json_object* keyJobj = NULL;
        keyJobj = static_cast<json_object*>(array_list_get_idx(keyList, arrIndex));

        if(keyJobj && json_object_is_type(keyJobj, json_type_string))
        {
            std::string clientKey(json_object_get_string(keyJobj));
            this->m_cKeyList.push_back(clientKey);
        }
    }

    json_object_put(jobj);

    if(this->m_cKeyList.empty() || this->m_processName.empty())
        return false;

    return true;
}

std::map<std::string, key_t> mcHubd::RegisterClientHandler::makeNewChannelList(mcHubd::Mediator* mediator)
{
    std::list<std::string>::iterator itor;
    std::map<std::string, key_t> keyChannelMap;
    mcHubd::Contract* contract = NULL;

    if(mediator)
    {
        // Each client sends its key list to create message channel
        // In this loop, new channel is created from the client key list

        int sequenceNumber = 0;

        for(itor = this->m_cKeyList.begin(); itor != this->m_cKeyList.end(); ++itor)
        {
            mcHubd::RESPCODE code;

            contract = new mcHubd::Contract(sequenceNumber);
            sequenceNumber++;

            if(contract)
            {
                contract->setProcessName(this->m_processName);
                contract->setProcessId(this->m_pid);
                contract->setClientKey((*itor));
                mediator->getNewChannel(&contract);

                if(contract->getRespCode() == MCHUBD_OK)
                {
                    key_t channel = contract->getChannel();
                    if(channel <= 0)
                    {
                        code = MCHUBD_CREATE_CHANNEL_ERROR;
                        mcHubd::MessageHandler::_responseError(code, (*itor), this->m_msg);
                        delete contract;
                        return keyChannelMap;;
                    }

                    keyChannelMap.insert(std::pair<std::string, key_t>((*itor), channel));
                }
                else
                {
                    mcHubd::MessageHandler::_responseError(contract->getRespCode(), (*itor), this->m_msg);
                    delete contract;
                    return keyChannelMap;;
                }

                delete contract;
            } //if contract
            else
            {
                code = MCHUBD_INTERNAL_ERROR;
                mcHubd::MessageHandler::_responseError(code, (*itor), this->m_msg);
                return keyChannelMap;;
            }
        }//for
    } // if mediator

    return keyChannelMap;;
}

void mcHubd::RegisterClientHandler::_responseOK(std::map<std::string, key_t> keyChannelMap, mcHubd::Message* msg)
{
    std::string response;
    mcHubd::RESPCODE code = MCHUBD_OK;
    struct json_object* jobj = NULL;
    struct json_object* msgTypeJobj = NULL;
    struct json_object* codeJobj = NULL;
    struct json_object* msgJobj = NULL;
    struct json_object* returnJobj = NULL;

    std::map<std::string, key_t>::iterator it;

    jobj = json_object_new_object();
    msgTypeJobj = json_object_new_string("RegisterClient");
    codeJobj = json_object_new_int(static_cast<int>(code));
    returnJobj = json_object_new_boolean(true);

    msgJobj = json_object_new_array();

    for(it = keyChannelMap.begin(); it != keyChannelMap.end(); ++it)
    {
        struct json_object* keyChannelJobj = json_object_new_object();
        struct json_object* keyJobj = json_object_new_string(it->first.c_str());
        struct json_object* channelJobj = json_object_new_int(static_cast<int>(it->second));

        json_object_object_add(keyChannelJobj, "key", keyJobj);
        json_object_object_add(keyChannelJobj, "channel", channelJobj);
        json_object_array_add(msgJobj, keyChannelJobj);
    }

    json_object_object_add(jobj, "msgType", msgTypeJobj);
    json_object_object_add(jobj, "code", codeJobj);
    json_object_object_add(jobj, "message", msgJobj);
    json_object_object_add(jobj, "return", returnJobj);

    response.assign(json_object_get_string(jobj));
    mcHubd::MessageTransportService::sendto(response, msg);
}
