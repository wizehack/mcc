#include <iostream>
#include "requestChannelTestSuite.h"
#include "baseFrameworkTestSuite.h"
#include "../mcHubType.h"
#include "../connectionInfo.h"
#include "../taskSet.h"
#include "../message.h"
#include "../manager.h"
#include "../mediator.h"
#include "../channelStatusMediator.h"
#include "../channelManager.h"
#include "../requestChannelHandler.h"
#include "../testStub.h"

std::string RequestChannelTestSuite::_testDataPath("");
std::string RequestChannelTestSuite::_configPath("");

RequestChannelTestSuite::RequestChannelTestSuite() : TestSuite(){}
RequestChannelTestSuite::~RequestChannelTestSuite(){}

bool RequestChannelTestSuite::request(TestOption* opt)
{
    if(opt == NULL)
        return false;

    if(opt->getCategory().compare("RequestChannel") == 0)
    {
        RequestChannelTestSuite::_testDataPath = opt->getTestDataPath();
        RequestChannelTestSuite::_configPath = opt->getConfigPath();
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
    mcHubd::Mediator* mediator = new DummyMediator();
    mcHubd::Manager* manager = new DummyManager(mediator, "Manager");
    manager->_setUpAcceptedList(RequestChannelTestSuite::_configPath);

    if(RequestChannelTestSuite::_setPrecondition())
    {
        this->add(1, RequestChannelTestSuite::_testRequestReadyChannel);
        this->add(2, RequestChannelTestSuite::_testRequestNotReadyChannel);
    }
    else
    {
        std::cout << "Precondition Failed" << std::endl;
    }

    delete manager;
    delete mediator;
}

bool RequestChannelTestSuite::_setPrecondition()
{
    struct json_object* jobj = NULL;
    struct json_object* psListJobj = NULL;
    struct array_list* psArrList = NULL;
    struct json_object* psNameJobj = NULL;
    struct json_object* pidJobj = NULL;
    struct json_object* keyListJobj = NULL;

    struct json_object* regListJobj = NULL;
    struct array_list* regList = NULL;

    int arrSize = 0;
    int arrIndex = 0;

    jobj = json_object_from_file(RequestChannelTestSuite::_testDataPath.c_str());

    if( (jobj == NULL) || is_error(jobj) )
    {
        std::cout << "Test Data is NOT JSON" << std::endl;
        return false;
    }

    if(!json_object_object_get_ex(jobj, "registeredKeyList", &regListJobj))
    {
        json_object_put(jobj);
        return false;
    }

    regList = json_object_get_array(regListJobj);

    if(regList == NULL)
    {
        json_object_put(jobj);
        return false;
    }

    arrSize = array_list_length(regList);

    for(arrIndex = 0; arrIndex < arrSize; arrIndex++)
    {
        std::string key;
        key_t channel;
        struct json_object* regItemJobj = NULL;
        struct json_object* keyJobj = NULL;
        struct json_object* channelJobj = NULL;

        regItemJobj = static_cast<json_object*>(array_list_get_idx(regList, arrIndex));

        if(!json_object_object_get_ex(regItemJobj, "key", &keyJobj))
        {
            json_object_put(jobj);
            return false;
        }

        if(!json_object_object_get_ex(regItemJobj, "channel", &channelJobj))
        {
            json_object_put(jobj);
            return false;
        }

        key.assign(json_object_get_string(keyJobj));
        channel = json_object_get_int(channelJobj);

        mcHubd::ConnectionInfo::getInstance()->addAvailableKey(key, channel);
        mcHubd::TaskSet::getInstance()->rmWaitToReady(key);
    }

    if(!json_object_object_get_ex(jobj, "connectedProcessList", &psListJobj))
    {
        json_object_put(jobj);
        return false;
    }

    psArrList = json_object_get_array(psListJobj);

    if(psArrList == NULL)
    {
        json_object_put(jobj);
        return false;
    }

    arrSize = array_list_length(psArrList);

    for(arrIndex = 0; arrIndex < arrSize; arrIndex++)
    {
        int pid;
        int keyArrIndex;
        int keyArrSize;
        struct json_object* psArrItemJobj = NULL;
        struct array_list* keyArrList = NULL;

        psArrItemJobj = static_cast<json_object*>(array_list_get_idx(psArrList, arrIndex));

        if(!json_object_object_get_ex(psArrItemJobj, "psname", &psNameJobj))
        {
            json_object_put(jobj);
            return false;
        }

        if(!json_object_object_get_ex(psArrItemJobj, "pid", &pidJobj))
        {
            json_object_put(jobj);
            return false;
        }

        pid = json_object_get_int(pidJobj);

        if(!json_object_object_get_ex(psArrItemJobj, "keyList", &keyListJobj))
        {
            json_object_put(jobj);
            return false;
        }

        keyArrList = json_object_get_array(keyListJobj);

        if(keyArrList == NULL)
        {
            json_object_put(jobj);
            return false;
        }

        keyArrSize = array_list_length(keyArrList);

        for(keyArrIndex = 0; keyArrIndex < keyArrSize; keyArrIndex++)
        {
            struct json_object* keyJobj = NULL;
            std::string clientKey;
            keyJobj = static_cast<json_object*>(array_list_get_idx(keyArrList, keyArrIndex));
            clientKey.assign(json_object_get_string(keyJobj));
            mcHubd::ConnectionInfo::getInstance()->addConnectedClientKeyMap(clientKey, static_cast<pid_t>(pid));
            mcHubd::TaskSet::getInstance()->addWaitToReadyTask(clientKey);
        }
    }

    json_object_put(jobj);
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
