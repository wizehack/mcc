#include "registerChannelHandler.h"
#include "clientManager.h"
#include "channelManager.h"
#include "channelStatusMediator.h"

mcHubd::RegisterChannelHandler::RegisterChannelHandler():
    m_channel(-1),
    m_cKey(){}
mcHubd::RegisterChannelHandler::~RegisterChannelHandler(){}

void mcHubd::RegisterChannelHandler::request(mcHubd::Message* msg)
{
    if(msg->getType() == REQ_REG_CHANNEL)
    {
        std::string respMsg;
        mcHubd::RESPCODE code;
        mcHubd::Mediator* mediator = NULL;
        std::shared_ptr<mcHubd::ClientManager> clientMgr;
        std::shared_ptr<mcHubd::ChannelManager> channelMgr;
        mcHubd::Contract* contract = NULL;

        if(this->parse(msg->getBody()) == false)
        {
            code = MCHUBD_INVALID_MSG;
            this->_responseError(code, respMsg);
            return;
        }

        mediator = new mcHubd::ChannelStatusMediator();

        if(mediator)
        {
            clientMgr = std::make_shared<mcHubd::ClientManager>(mediator);
            channelMgr = std::make_shared<mcHubd::ChannelManager>(mediator);

            mediator->appendManager(clientMgr);
            mediator->appendManager(channelMgr);

            /* register channel */
            contract = new mcHubd::Contract();
            contract->setChannel(this->m_channel);
            contract->setClientKey(this->m_cKey);

            mediator->registerNewChannel(&contract);

            if(contract)
            {
                if(contract->getRespCode() == MCHUBD_OK)
                {
                    struct json_object* jobj = NULL;

                    jobj = json_object_new_object();
                    if(mcHubd::RegisterChannelHandler::_makeResponseMessage(&jobj, this->m_cKey, this->m_channel) == false)
                    {
                        code = MCHUBD_INFORM_CHANNEL_ERROR;
                        this->_responseError(code, this->m_cKey);
                        json_object_put(jobj);
                        delete contract;
                        delete mediator;
                        return;
                    }

                    respMsg.assign(json_object_get_string(jobj));
                    this->_responseOK(respMsg);

                    json_object_put(jobj);
                }
                else
                {
                    respMsg.clear();
                    this->_responseError(contract->getRespCode(), respMsg);
                }

                delete contract;
                delete mediator;
            }
        }
    }
    else
    {
        if(this->m_next)
            this->m_next->request(msg);
    }
}

bool mcHubd::RegisterChannelHandler::parse(std::string payload)
{
    struct json_object* jobj = NULL;
    struct json_object* channelJobj = NULL;
    struct json_object* clientKeyJobj = NULL;

    //initialize member variables
    this->m_cKey.clear();
    this->m_channel = -1;

    if(payload.empty())
        return false;

    jobj = json_tokener_parse(payload.c_str());

    if((jobj == NULL) || is_error(jobj))
        return false;

    if(!json_object_object_get_ex(jobj, "key", &clientKeyJobj))
    {
        json_object_put(jobj);
        return false;
    }

    if(!json_object_object_get_ex(jobj, "channel", &channelJobj))
    {
        json_object_put(jobj);
        return false;
    }

    this->m_cKey.assign(json_object_get_string(clientKeyJobj));
    this->m_channel = static_cast<key_t>(json_object_get_int(channelJobj));
    json_object_put(jobj);
    return true;
}

bool mcHubd::RegisterChannelHandler::_makeResponseMessage(struct json_object** pJobj, std::string cKey, key_t channel)
{
    struct json_object* jobj;
    struct json_object* keyJobj;
    struct json_object* channelJobj;
    struct json_object* stateJobj;

    jobj = (*pJobj);

    if(jobj == NULL)
        return false;

    if(cKey.empty() == true)
        return false;

    if(channel < 0)
        return false;

    keyJobj = json_object_new_string(cKey.c_str());
    channelJobj = json_object_new_int(static_cast<int>(channel));
    stateJobj = json_object_new_string("ready");

    json_object_object_add(jobj, "key", keyJobj);
    json_object_object_add(jobj, "channel", channelJobj);
    json_object_object_add(jobj, "state", stateJobj);

    return true;
}
