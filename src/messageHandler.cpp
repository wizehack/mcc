#include "messageHandler.h"

mcHubd::MessageHandler::MessageHandler():m_next(NULL){}

mcHubd::MessageHandler::~MessageHandler(){}

void mcHubd::MessageHandler::setNext(mcHubd::MessageHandler* handler)
{
    if(handler)
        this->m_next = handler;
    else
        std::cout << __FUNCTION__ << " [" << __LINE__ << "]: " << "handler is NULL" << std::endl;
}

void mcHubd::MessageHandler::responseError(mcHubd::RESPCODE code, std::string extraMsg)
{
    struct json_object* jobj = NULL;
    struct json_object* codeJobj = NULL;
    struct json_object* msgJobj = NULL;
    struct json_object* extraJobj = NULL;
    struct json_object* returnJobj = NULL;

    jobj = json_object_new_object();
    codeJobj = json_object_new_int(static_cast<int>(code));
    msgJobj = json_object_new_string(this->getMessage(code).c_str());
    extraJobj = json_object_new_string(extraMsg.c_str());
    returnJobj = json_object_new_boolean(false);

    json_object_object_add(jobj, "errorCode", codeJobj);
    json_object_object_add(jobj, "errorMessage", msgJobj);
    json_object_object_add(jobj, "extraMessage", extraJobj);
    json_object_object_add(jobj, "return", returnJobj);

    std::cout << __FUNCTION__ << " : " << json_object_get_string(jobj) << std::endl;
    json_object_put(jobj);
}

void mcHubd::MessageHandler::responseOK(std::string respMsg)
{
    mcHubd::RESPCODE code = MCHUBD_OK;
    struct json_object* jobj = NULL;
    struct json_object* codeJobj = NULL;
    struct json_object* msgJobj = NULL;
    struct json_object* returnJobj = NULL;

    jobj = json_object_new_object();
    codeJobj = json_object_new_int(static_cast<int>(code));
    msgJobj = json_object_new_string(respMsg.c_str());
    returnJobj = json_object_new_boolean(true);

    json_object_object_add(jobj, "code", codeJobj);
    json_object_object_add(jobj, "message", msgJobj);
    json_object_object_add(jobj, "return", returnJobj);

    std::cout << __FUNCTION__ << " : " << json_object_get_string(jobj) << std::endl;
    json_object_put(jobj);
}


std::string mcHubd::MessageHandler::getMessage(mcHubd::RESPCODE code)
{
    std::string message;

    if(code == MCHUBD_OK)
        message.assign("OK");
    else if (code == MCHUBD_INVALID_MSG)
        message.assign("INVALID MESSAGE");
    else if (code == MCHUBD_NOT_ACCEPTABLE_KEY)
        message.assign("NOT ACCEPTABLE KEY");
    else if (code == MCHUBD_IS_CREATED_KEY)
        message.assign("CREATED KEY");
    else if (code == MCHUBD_IS_AVAILABLE_KEY)
        message.assign("AVAILABLE KEY");
    else if (code == MCHUBD_IS_NOT_AVAILABLE_KEY)
        message.assign("NOT AVAILABLE KEY");
    else if (code == MCHUBD_CREATE_CHANNEL_ERROR)
        message.assign("CREATE CHANNEL ERROR");
    else if (code == MCHUBD_IS_INFORMED_KEY)
        message.assign("INFORMED KEY");
    else if (code == MCHUBD_INFORM_CHANNEL_ERROR)
        message.assign("INFORM CHANNEL ERROR");
    else if (code == MCHUBD_EXCEEDED_MAXIMUM_CLIENT_KEY)
        message.assign("EXCEEDED MAXIMUM CLIENT KEY");
    else if (code == MCHUBD_INTERNAL_ERROR)
        message.assign("INTERNAL ERROR");

    return message;
}
