#include <iostream>
#include "requestChannelTestSuite.h"
#include "testData.h"
#include "reqMsgMaker.h"
#include "udpClient.h"

std::string DeleteChannelTestSuite::_testDataPath("");
std::string DeleteChannelTestSuite::_psName("");
std::list<std::string> DeleteChannelTestSuite::_keyList;

RequestChannelTestSuite::RequestChannelTestSuite() : TestSuite(){}
RequestChannelTestSuite::~RequestChannelTestSuite(){}

bool RequestChannelTestSuite::request(TestOption* opt)
{
    if(opt == NULL)
        return false;

    if(opt->getCategory().compare("RequestChannel") == 0)
    {
        RequestChannelTestSuite::_testDataPath = opt->getTestDataPath();
        this->registerTestCase();
        return this->exec(opt->getTestCaseID());
    }

    if(this->m_next)
    {
        return this->m_next->request(opt);
    }

    return false;
}

void RequestChannelTestSuite::registerTestCase()
{
    if(RequestChannelTestSuite::_setPrecondition())
    {
        this->add(1, RequestChannelTestSuite::_test);
    }
    else
    {
        std::cout << "Precondition Failed" << std::endl;
    }
}

bool RequestChannelTestSuite::_setPrecondition()
{
    return true;
}

bool RequestChannelTestSuite::_verifyResponseOk(struct json_object* jobj, std::string key, std::string state, int channel)
{
    struct json_object* codeJobj = NULL;
    struct json_object* retJobj = NULL;
    struct json_object* messageJobj = NULL;
    struct json_object* keyJobj = NULL;
    struct json_object* channelJobj = NULL;
    struct json_object* stateJobj = NULL;
    mcHubd::RESPCODE code;
    std::string state;

    if((jobj == NULL) || is_error(jobj))
        return false;

    if(!json_object_object_get_ex(jobj, "code", &codeJobj))
    {
        json_object_put(jobj);
        return false;
    }

    code = static_cast<mcHubd::RESPCODE>(json_object_get_int(codeJobj));
    if(code != mcHubd::MCHUBD_OK)
    {
        json_object_put(jobj);
        return false;
    }

    if(!json_object_object_get_ex(jobj, "return", &retJobj))
    {
        json_object_put(jobj);
        return false;
    }

    if(!json_object_get_boolean(retJobj))
    {
        json_object_put(jobj);
        return false;
    }

    if(!json_object_object_get_ex(jobj, "message", &messageJobj))
    {
        json_object_put(jobj);
        return false;
    }

    if(!json_object_object_get_ex(messageJobj, "key", &keyJobj))
    {
        json_object_put(jobj);
        return false;
    }

    if(!json_object_object_get_ex(messageJobj, "channel", &channelJobj))
    {
        json_object_put(jobj);
        return false;
    }

    if(!json_object_object_get_ex(messageJobj, "state", &stateJobj))
    {
        json_object_put(jobj);
        return false;
    }

    state.assign(json_object_get_string(stateJobj));

    if(!keyJobj || !channelJobj || !stateJobj)
    {
        json_object_put(jobj);
        return false;
    }
    else
    {
        std::string resultKey(json_object_get_string(keyJobj));
        std::string resultState(json_object_get_string(stateJobj));
        int resultChannel = json_object_get_int(channelJobj);

        if(key.compare(resultKey) != 0)
        {
            json_object_put(jobj);
            return false;
        }

        if(state.compare(resultState) != 0)
        {
            json_object_put(jobj);
            return false;
        }

        if(channel != resultChannel)
        {
            json_object_put(jobj);
            return false;
        }
    }

    json_object_put(jobj);
    return true;
}

bool RequestChannelTestSuite::_testRequestReadyChannel()
{
    mcHubd::RequestChannelHandler handler;
    std::shared_ptr<mcHubd::Message> sptrMsg = std::make_shared<mcHubd::Message>(mcHubd::REQ_GET_CHANNEL);
    mcHubd::Message* msg = sptrMsg.get();
    std::string state("ready");
    std::string body;
    std::string key;
    key.assign("com.mchannel.test.t3");
    body.assign("{\"key\": \"com.mchannel.test.t3\"}");
    msg->setBody(body);
    if(handler.request(msg) == false)
        return false;

    struct json_object* jobj = json_tokener_parse(mcHubd::TestStub::getInstance()->getRespMsg(0).c_str());
    bool isPassed = RequestChannelTestSuite::_verifyResponseOk(jobj, key, state, 1000);
    json_object_put(jobj);

    if(isPassed == false)
        return false;

    key.assign("com.mchannel.test.t4");
    body.assign("{\"key\": \"com.mchannel.test.t4\"}");
    msg->setBody(body);
    if(handler.request(msg) == false)
        return false;

    jobj = json_tokener_parse(mcHubd::TestStub::getInstance()->getRespMsg(1).c_str());
    isPassed = RequestChannelTestSuite::_verifyResponseOk(jobj, key, state, 1001);
    json_object_put(jobj);

    if(isPassed == false)
        return false;

    return true;
}

bool RequestChannelTestSuite::_testRequestNotReadyChannel()
{
    std::string respMessge("NOT AVAILABLE KEY");
    mcHubd::RESPCODE code = mcHubd::MCHUBD_IS_NOT_AVAILABLE_KEY;
    struct json_object* jobj = NULL;

    mcHubd::RequestChannelHandler handler;
    std::shared_ptr<mcHubd::Message> sptrMsg = std::make_shared<mcHubd::Message>(mcHubd::REQ_GET_CHANNEL);
    mcHubd::Message* msg = sptrMsg.get();

    std::string body;
    std::string key;
    key.assign("com.mchannel.test.t1");
    body.assign("{\"key\": \"com.mchannel.test.t1\"}");
    msg->setBody(body);
    if(handler.request(msg) == true)
        return false;

    jobj = json_tokener_parse(mcHubd::TestStub::getInstance()->getRespMsg(0).c_str());

    if(RequestChannelTestSuite::_verifyResponseError(jobj, code, respMessge) == false)
    {
        json_object_put(jobj);
        return false;
    }

    key.assign("com.mchannel.test.t0");
    body.assign("{\"key\": \"com.mchannel.test.t0\"}");
    msg->setBody(body);
    if(handler.request(msg) == true)
        return false;

    jobj = json_tokener_parse(mcHubd::TestStub::getInstance()->getRespMsg(1).c_str());

    if(RequestChannelTestSuite::_verifyResponseError(jobj, code, respMessge) == false)
    {
        json_object_put(jobj);
        return false;
    }

    json_object_put(jobj);
    return true;
}
