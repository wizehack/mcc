#include "requestChannelHandler.h"
#include "clientManager.h"
#include "channelManager.h"
#include "channelStatusMediator.h"

mcHubd::RequestChannelHandler::RequestChannelHandler():m_cKey(){}
mcHubd::RequestChannelHandler::~RequestChannelHandler(){}

bool mcHubd::RequestChannelHandler::request(mcHubd::Message* msg)
{
    if(msg->getType() == REQ_GET_CHANNEL)
    {
        bool ret = false;
        std::string respMsg;
        mcHubd::RESPCODE code;
        mcHubd::Mediator* mediator = NULL;

        if(this->parse(msg->getBody()) == false)
        {
            mcHubd::RESPCODE code;
            code = MCHUBD_INVALID_MSG;

            this->_responseError(code, respMsg);
            return false;
        }

        mediator = new ChannelStatusMediator();

        if(mediator)
        {
            mcHubd::Contract* contract = new mcHubd::Contract();
            contract->setClientKey(this->m_cKey);

            std::shared_ptr<mcHubd::ChannelManager> channelMgr
                = std::make_shared<mcHubd::ChannelManager>(mediator);
            std::shared_ptr<mcHubd::ClientManager> clientMgr
                = std::make_shared<mcHubd::ClientManager>(mediator);

            mediator->appendManager(clientMgr);
            mediator->appendManager(channelMgr);
            mediator->getChannel(&contract);

            code = contract->getRespCode();

            if(code != MCHUBD_OK)
            {
                this->_responseError(code, respMsg);
            }
            else
            {
                int channel = static_cast<int>(contract->getChannel());
                std::string status = mcHubd::ChannelStatusMediator::getChannelStatus(contract);
                struct json_object* jobj = NULL;
                jobj = json_object_new_object();

                if(mcHubd::RequestChannelHandler::_makeResponseMessage(&jobj, this->m_cKey, channel, status) == false)
                {
                    code = MCHUBD_INTERNAL_ERROR;
                    this->_responseError(code, this->m_cKey);
                    json_object_put(jobj);
                    delete contract;
                    delete mediator;
                    return false;
                }

                respMsg.assign(json_object_get_string(jobj));
                this->_responseOK(respMsg);
                ret = true;
                json_object_put(jobj);
            }

            delete contract;
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

bool mcHubd::RequestChannelHandler::parse(std::string payload)
{
    struct json_object* jobj = NULL;
    struct json_object* keyJobj = NULL;

    jobj = json_tokener_parse(payload.c_str());

    if((jobj == NULL) || is_error(jobj))
        return false;

    if(!json_object_object_get_ex(jobj, "key", &keyJobj))
    {
        json_object_put(jobj);
        return false;
    }

    if(keyJobj)
    {
        this->m_cKey.assign(json_object_get_string(keyJobj));
        return true;
    }

    return false;
}

bool mcHubd::RequestChannelHandler::_makeResponseMessage(struct json_object** pJobj, std::string cKey, key_t channel, std::string status)
{
    struct json_object* jobj = (*pJobj);

    if(jobj)
    {
        struct json_object* keyJobj = NULL;
        struct json_object* channelJobj = NULL;
        struct json_object* stateJobj = NULL;

        keyJobj = json_object_new_string(cKey.c_str());
        channelJobj = json_object_new_int(static_cast<int>(channel));
        stateJobj = json_object_new_string(status.c_str());

        json_object_object_add(jobj, "key", keyJobj);
        json_object_object_add(jobj, "channel", channelJobj);
        json_object_object_add(jobj, "state", stateJobj);
        return true;
    }

    return false;
}
