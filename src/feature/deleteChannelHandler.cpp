#include "deleteChannelHandler.h"
#include "channelManager.h"
#include "clientManager.h"
#include "channelStatusMediator.h"

mcHubd::DeleteChannelHandler::DeleteChannelHandler():
    m_cKey(),
    m_channel(-1),
    m_msg(NULL){}
mcHubd::DeleteChannelHandler::~DeleteChannelHandler(){}

bool mcHubd::DeleteChannelHandler::request(mcHubd::Message* msg)
{
    if(msg->getType() == REQ_DEL_CHANNEL)
    {
        bool ret = false;
        std::string respMsg;
        mcHubd::RESPCODE code;
        mcHubd::Mediator* mediator = NULL;
        this->m_msg = msg;

        if(this->parse(msg->getBody()) == false)
        {
            mcHubd::RESPCODE code;
            code = MCHUBD_INVALID_MSG;

            this->_responseError(code, respMsg, this->m_msg);
            return false;
        }

        mediator = new ChannelStatusMediator();

        if(mediator)
        {
            mcHubd::Contract* contract = new mcHubd::Contract();

            contract->setClientKey(this->m_cKey);
            contract->setChannel(this->m_channel);

            std::shared_ptr<mcHubd::ChannelManager> channelMgr
                = std::make_shared<mcHubd::ChannelManager>(mediator);
            std::shared_ptr<mcHubd::ClientManager> clientMgr
                = std::make_shared<mcHubd::ClientManager>(mediator);

            mediator->appendManager(clientMgr);
            mediator->appendManager(channelMgr);
            mediator->deleteChannel(&contract);

            code = contract->getRespCode();
            if(code != MCHUBD_OK)
            {
                this->_responseError(code, respMsg, this->m_msg);
            }
            else
            {
                struct json_object* jobj = NULL;
                jobj = json_object_new_object();
                if(mcHubd::DeleteChannelHandler::_makeResponseMessage(&jobj, this->m_cKey, this->m_channel) == false)
                {
                    code = MCHUBD_INTERNAL_ERROR;
                    this->_responseError(code, this->m_cKey, this->m_msg);
                    json_object_put(jobj);
                    delete contract;
                    delete mediator;
                    return false;
                }

                respMsg.assign(json_object_get_string(jobj));
                this->_responseOK(respMsg, this->m_msg);
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

bool mcHubd::DeleteChannelHandler::parse(std::string payload)
{
    struct json_object* jobj = NULL;
    struct json_object* clientKeyJobj = NULL;
    struct json_object* channelJobj = NULL;

    //initialize member variables
    this->m_cKey.clear();

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

    this->m_cKey.assign(json_object_get_string(clientKeyJobj)); 

    if(!json_object_object_get_ex(jobj, "channel", &channelJobj))
    {
        //Nothing, this is an optional key
    }

    if(channelJobj)
        this->m_channel = static_cast<key_t>(json_object_get_int(channelJobj));

    json_object_put(jobj);

    return true;
}

bool mcHubd::DeleteChannelHandler::_makeResponseMessage(struct json_object** pJobj, std::string cKey, key_t channel)
{
    struct json_object* jobj;
    struct json_object* keyJobj;
    struct json_object* stateJobj;

    jobj = (*pJobj);

    if(jobj == NULL)
        return false;

    if(cKey.empty())
        return false;

    keyJobj = json_object_new_string(cKey.c_str());
    stateJobj = json_object_new_string("closed");

    json_object_object_add(jobj, "key", keyJobj);
    json_object_object_add(jobj, "state", stateJobj);

    if(channel > 0)
    {
        struct json_object* channelJobj;
        channelJobj = json_object_new_int(static_cast<int>(channel));
        json_object_object_add(jobj, "channel", channelJobj);
    }

    return true;
}
