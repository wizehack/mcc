#include <iostream>
#include "mcHubType.h"
#include "feature/message.h"
#include "feature/unknownMessageHandler.h"
#include "undefinedTypeTestSuite.h"
#include "../testStub.h"

UndefinedTypeTestSuite::UndefinedTypeTestSuite() :
    TestSuite(){}
UndefinedTypeTestSuite::~UndefinedTypeTestSuite(){}

bool UndefinedTypeTestSuite::request(TestOption* opt)
{
    if(opt == NULL)
        return false;

    if(opt->getCategory().compare("UndefinedType") == 0)
    {
        this->registerTestCase();
        return this->exec(opt->getTestCaseID());
    }

    if(this->m_next)
    {
        return this->m_next->request(opt);
    }

    return false;
}


void UndefinedTypeTestSuite::registerTestCase()
{
    this->add(1, UndefinedTypeTestSuite::_test);
}

bool UndefinedTypeTestSuite::_test()
{
    mcHubd::UnknownMessageHandler handler;
    std::shared_ptr<mcHubd::Message> sptrMsg = std::make_shared<mcHubd::Message>(mcHubd::UNKNOWN_MSG_TYPE);
    mcHubd::Message* msg = sptrMsg.get();

    std::string body;
    body.assign("{\"key\": \"com.mchannel.foo.f1\", \"channel\": 2000}");
    msg->setBody(body);

    if(handler.request(msg) == true)
        return false;

    struct json_object* jobj = json_tokener_parse(mcHubd::TestStub::getInstance()->getRespMsg(0).c_str());

    if((jobj == NULL) || is_error(jobj))
        return false;

    bool isPassed = UndefinedTypeTestSuite::_verifyResponse(jobj);
    json_object_put(jobj);
    return isPassed;

    return false;
}

bool UndefinedTypeTestSuite::_verifyResponse(struct json_object* jobj)
{
    struct json_object* codeJobj = NULL;
    struct json_object* retJobj = NULL;
    mcHubd::RESPCODE code;

    if((jobj == NULL) || is_error(jobj))
        return false;

    if(!json_object_object_get_ex(jobj, "errorCode", &codeJobj))
    {
        json_object_put(jobj);
        return false;
    }

    code = static_cast<mcHubd::RESPCODE>(json_object_get_int(codeJobj));
    if(code != mcHubd::MCHUBD_UNDEFINED_TYPE)
    {
        json_object_put(jobj);
        return false;
    }

    if(!json_object_object_get_ex(jobj, "return", &retJobj))
    {
        json_object_put(jobj);
        return false;
    }

    if(json_object_get_boolean(retJobj))
    {
        json_object_put(jobj);
        return false;
    }

    json_object_put(jobj);
    return true;
}
