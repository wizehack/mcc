#include "deleteClientHandler.h"
#include "channelManager.h"
#include "clientManager.h"
#include "channelStatusMediator.h"

mcHubd::DeleteClientHandler::DeleteClientHandler():
    m_pid(-1),
    m_processName(){}
mcHubd::DeleteClientHandler::~DeleteClientHandler(){}

bool mcHubd::DeleteClientHandler::request(mcHubd::Message* msg)
{
    if(msg->getType() == REQ_DEL_CLIENT)
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

            contract->setProcessName(this->m_processName);
            contract->setProcessId(this->m_pid);

            std::shared_ptr<mcHubd::ChannelManager> channelMgr
                = std::make_shared<mcHubd::ChannelManager>(mediator);
            std::shared_ptr<mcHubd::ClientManager> clientMgr
                = std::make_shared<mcHubd::ClientManager>(mediator);

            mediator->appendManager(clientMgr);
            mediator->appendManager(channelMgr);
            mediator->deleteClient(&contract);

            code = contract->getRespCode();
            if(code != MCHUBD_OK)
            {
                this->_responseError(code, respMsg);
            }
            else
            {
                struct json_object* jobj = NULL;
                jobj = json_object_new_object();
                if(mcHubd::DeleteClientHandler::_makeResponseMessage(&jobj, this->m_processName, this->m_pid) == false)
                {
                    code = MCHUBD_INTERNAL_ERROR;
                    this->_responseError(code, respMsg);
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

bool mcHubd::DeleteClientHandler::parse(std::string payload)
{
    struct json_object* jobj = NULL;
    struct json_object* processIdJobj = NULL;
    struct json_object* processNameJobj = NULL;

    //initialize member variables
    this->m_pid = -1;
    this->m_processName.clear();

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

    json_object_put(jobj);

    return true;
}

bool mcHubd::DeleteClientHandler::_makeResponseMessage(struct json_object** pJobj, std::string psName, pid_t pid)
{
    struct json_object* jobj = NULL;
    struct json_object* psNameJobj = NULL;
    struct json_object* pidJobj = NULL;

    jobj = (*pJobj);

    if(jobj == NULL)
        return false;

    if(psName.empty())
        return false;

    if(pid <= 0)
        return false;

    psNameJobj = json_object_new_string(psName.c_str());
    pidJobj = json_object_new_int(static_cast<int>(pid));

    json_object_object_add(jobj, "psName", psNameJobj);
    json_object_object_add(jobj, "pid", pidJobj);

    return true;
}
