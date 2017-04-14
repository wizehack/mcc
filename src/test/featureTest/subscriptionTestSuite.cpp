#include <iostream>
#include "subscriptionTestSuite.h"

#include "mcHubType.h"
#include "feature/message.h"
#include "feature/manager.h"
#include "feature/clientManager.h"
#include "feature/mediator.h"
#include "feature/channelStatusMediator.h"
#include "feature/testStub.h"
#include "feature/taskSet.h"
#include "feature/deleteChannelHandler.h"
#include "feature/deleteClientHandler.h"
#include "feature/registerChannelHandler.h"
#include "feature/registerClientHandler.h"

std::string SubscriptionTestSuite::_testDataPath("");

SubscriptionTestSuite::SubscriptionTestSuite() : TestSuite(){}
SubscriptionTestSuite::~SubscriptionTestSuite(){}

bool SubscriptionTestSuite::request(TestOption* opt)
{
    if(opt == NULL)
        return false;

    if(opt->getCategory().compare("SubscribeChannelStatus") == 0)
    {
        SubscriptionTestSuite::_testDataPath = opt->getTestDataPath();
        this->registerTestCase();
        return this->exec(opt->getTestCaseID());
    }

    if(this->m_next)
    {
        return this->m_next->request(opt);
    }

    return false;
}

void SubscriptionTestSuite::registerTestCase()
{
    if(SubscriptionTestSuite::_setPrecondition())
    {
        this->add(1, SubscriptionTestSuite::_testChannelOpen);
        this->add(2, SubscriptionTestSuite::_testChannelReady);
        this->add(3, SubscriptionTestSuite::_testChannelCloseByDeleteChannel);
        this->add(4, SubscriptionTestSuite::_testChannelCloseByDeleteClient);
    }
    else
    {
        std::cout << "precondition error" << std::endl;
    }
}

bool SubscriptionTestSuite::_testChannelOpen()
{
    //register client
    mcHubd::RegisterClientHandler regCliHandler;
    std::shared_ptr<mcHubd::Message> sptrMsg = std::make_shared<mcHubd::Message>(mcHubd::REQ_REG_CLIENT);
    mcHubd::Message* msg = sptrMsg.get();
    struct json_object* jobj = NULL;
    std::string state("open");
    std::string response;
    std::string body;
    std::string key("com.mchannel.test.t1");
    body.assign("{\"pid\": 1212, \"psName\": \"test\", \"keyList\":");
    body.append("[\"com.mchannel.test.t1\"]");
    body.append("}");
    msg->setBody(body);
    regCliHandler.request(msg);

    response = mcHubd::TestStub::getInstance()->getSubscribepMsg(0);
    jobj = json_tokener_parse(response.c_str());

    return SubscriptionTestSuite::_verifyResponse(jobj, key, 0, state);
}

bool SubscriptionTestSuite::_testChannelReady()
{
    //register Channel
    mcHubd::RegisterChannelHandler regChannelHandler;
    std::shared_ptr<mcHubd::Message> sptrMsg = std::make_shared<mcHubd::Message>(mcHubd::REQ_REG_CHANNEL);
    mcHubd::Message* msg = sptrMsg.get();
    std::string body;

    std::string response;
    struct json_object* jobj = NULL;
    std::string state("ready");

    /*registered key*/
    std::string t1("com.mchannel.test.t1");
    std::string t2("com.mchannel.test.t2");
    std::string t3("com.mchannel.test.t3");
    std::string t4("com.mchannel.test.t4");
    std::string t5("com.mchannel.test.t5");
    std::string t6("com.mchannel.test.t6");
    std::string t7("com.mchannel.test.t7");
    std::string t8("com.mchannel.test.t8");
    std::string t9("com.mchannel.test.t9");
    std::string t10("com.mchannel.test.t10");

    body.assign("{\"key\": \"com.mchannel.test.t1\", \"channel\": 1000}");
    msg->setBody(body);
    regChannelHandler.request(msg);

    response = mcHubd::TestStub::getInstance()->getSubscribepMsg(0);
    jobj = json_tokener_parse(response.c_str());
    if(SubscriptionTestSuite::_verifyResponse(jobj, t1, 1000, state) == false)
        return false;
    json_object_put(jobj);

    body.assign("{\"key\": \"com.mchannel.test.t2\", \"channel\": 1001}");
    msg->setBody(body);
    regChannelHandler.request(msg);

    response = mcHubd::TestStub::getInstance()->getSubscribepMsg(1);
    jobj = json_tokener_parse(response.c_str());
    if(SubscriptionTestSuite::_verifyResponse(jobj, t2, 1001, state) == false)
        return false;
    json_object_put(jobj);

    body.assign("{\"key\": \"com.mchannel.test.t3\", \"channel\": 1002}");
    msg->setBody(body);
    regChannelHandler.request(msg);

    response = mcHubd::TestStub::getInstance()->getSubscribepMsg(2);
    jobj = json_tokener_parse(response.c_str());
    if(SubscriptionTestSuite::_verifyResponse(jobj, t3, 1002, state) == false)
        return false;
    json_object_put(jobj);

    body.assign("{\"key\": \"com.mchannel.test.t4\", \"channel\": 1003}");
    msg->setBody(body);
    regChannelHandler.request(msg);

    response = mcHubd::TestStub::getInstance()->getSubscribepMsg(3);
    jobj = json_tokener_parse(response.c_str());
    if(SubscriptionTestSuite::_verifyResponse(jobj, t4, 1003, state) == false)
        return false;
    json_object_put(jobj);

    body.assign("{\"key\": \"com.mchannel.test.t5\", \"channel\": 1004}");
    msg->setBody(body);
    regChannelHandler.request(msg);

    response = mcHubd::TestStub::getInstance()->getSubscribepMsg(4);
    jobj = json_tokener_parse(response.c_str());
    if(SubscriptionTestSuite::_verifyResponse(jobj, t5, 1004, state) == false)
        return false;
    json_object_put(jobj);

    body.assign("{\"key\": \"com.mchannel.test.t6\", \"channel\": 1005}");
    msg->setBody(body);
    regChannelHandler.request(msg);

    response = mcHubd::TestStub::getInstance()->getSubscribepMsg(5);
    jobj = json_tokener_parse(response.c_str());
    if(SubscriptionTestSuite::_verifyResponse(jobj, t6, 1005, state) == false)
        return false;
    json_object_put(jobj);

    body.assign("{\"key\": \"com.mchannel.test.t7\", \"channel\": 1006}");
    msg->setBody(body);
    regChannelHandler.request(msg);

    response = mcHubd::TestStub::getInstance()->getSubscribepMsg(6);
    jobj = json_tokener_parse(response.c_str());
    if(SubscriptionTestSuite::_verifyResponse(jobj, t7, 1006, state) == false)
        return false;
    json_object_put(jobj);

    body.assign("{\"key\": \"com.mchannel.test.t8\", \"channel\": 1007}");
    msg->setBody(body);
    regChannelHandler.request(msg);

    response = mcHubd::TestStub::getInstance()->getSubscribepMsg(7);
    jobj = json_tokener_parse(response.c_str());
    if(SubscriptionTestSuite::_verifyResponse(jobj, t8, 1007, state) == false)
        return false;
    json_object_put(jobj);

    body.assign("{\"key\": \"com.mchannel.test.t9\", \"channel\": 1008}");
    msg->setBody(body);
    regChannelHandler.request(msg);

    response = mcHubd::TestStub::getInstance()->getSubscribepMsg(8);
    jobj = json_tokener_parse(response.c_str());
    if(SubscriptionTestSuite::_verifyResponse(jobj, t9, 1008, state) == false)
        return false;
    json_object_put(jobj);

    body.assign("{\"key\": \"com.mchannel.test.t10\", \"channel\": 1009}");
    msg->setBody(body);
    regChannelHandler.request(msg);

    response = mcHubd::TestStub::getInstance()->getSubscribepMsg(9);
    jobj = json_tokener_parse(response.c_str());
    if(SubscriptionTestSuite::_verifyResponse(jobj, t10, 1009, state) == false)
        return false;
    json_object_put(jobj);

    return true;
}

bool SubscriptionTestSuite::_testChannelCloseByDeleteChannel()
{
    mcHubd::DeleteChannelHandler handler;
    std::shared_ptr<mcHubd::Message> sptrMsg = std::make_shared<mcHubd::Message>(mcHubd::REQ_DEL_CHANNEL);
    mcHubd::Message* msg = sptrMsg.get();

    struct json_object* jobj = NULL;
    std::string state("closed");
    std::string response;
    std::string body;
    std::string f1("com.mchannel.foo.f1");
    std::string f2("com.mchannel.foo.f2");

    body.assign("{\"key\": \"com.mchannel.foo.f1\", \"channel\": 2000}");
    msg->setBody(body);
    handler.request(msg);

    response = mcHubd::TestStub::getInstance()->getSubscribepMsg(0);
    jobj = json_tokener_parse(response.c_str());
    if(SubscriptionTestSuite::_verifyResponse(jobj, f1, 2000, state) == false)
        return false;
    json_object_put(jobj);

    body.assign("{\"key\": \"com.mchannel.foo.f2\", \"channel\": 2001}");
    msg->setBody(body);
    handler.request(msg);

    response = mcHubd::TestStub::getInstance()->getSubscribepMsg(1);
    jobj = json_tokener_parse(response.c_str());
    if(SubscriptionTestSuite::_verifyResponse(jobj, f2, 2001, state) == false)
        return false;
    json_object_put(jobj);

    return true;
}

bool SubscriptionTestSuite::_testChannelCloseByDeleteClient()
{
    mcHubd::DeleteClientHandler handler;
    std::shared_ptr<mcHubd::Message> sptrMsg = std::make_shared<mcHubd::Message>(mcHubd::REQ_DEL_CLIENT);
    mcHubd::Message* msg = sptrMsg.get();
    std::string body;

    struct json_object* jobj = NULL;
    std::string state("closed");
    std::string response;
    std::string t1("com.mchannel.test.t1");
    std::string t2("com.mchannel.test.t2");
    std::string f1("com.mchannel.foo.f1");
    std::string f2("com.mchannel.foo.f2");
    std::string b1("com.mchannel.bar.b1");
    std::string b2("com.mchannel.bar.b2");

    body.assign("{\"pid\": 100, \"psName\": \"test\"}");
    msg->setBody(body);
    handler.request(msg);

    response = mcHubd::TestStub::getInstance()->getSubscribepMsg(0);
    jobj = json_tokener_parse(response.c_str());
    if(SubscriptionTestSuite::_verifyResponse(jobj, t1, 0, state) == false)
        return false;

    response = mcHubd::TestStub::getInstance()->getSubscribepMsg(1);
    jobj = json_tokener_parse(response.c_str());
    if(SubscriptionTestSuite::_verifyResponse(jobj, t2, 0, state) == false)
        return false;

    body.assign("{\"pid\": 200, \"psName\": \"foo\"}");
    msg->setBody(body);
    handler.request(msg);

    response = mcHubd::TestStub::getInstance()->getSubscribepMsg(2);
    jobj = json_tokener_parse(response.c_str());
    if(SubscriptionTestSuite::_verifyResponse(jobj, f1, 2000, state) == false)
        return false;

    response = mcHubd::TestStub::getInstance()->getSubscribepMsg(3);
    jobj = json_tokener_parse(response.c_str());
    if(SubscriptionTestSuite::_verifyResponse(jobj, f2, 2001, state) == false)
        return false;

    body.assign("{\"pid\": 400, \"psName\": \"bar\"}");
    msg->setBody(body);
    handler.request(msg);

    response = mcHubd::TestStub::getInstance()->getSubscribepMsg(4);
    jobj = json_tokener_parse(response.c_str());
    if(SubscriptionTestSuite::_verifyResponse(jobj, b1, 0, state) == false)
        return false;

    response = mcHubd::TestStub::getInstance()->getSubscribepMsg(5);
    jobj = json_tokener_parse(response.c_str());
    if(SubscriptionTestSuite::_verifyResponse(jobj, b2, 0, state) == false)
        return false;

    return true;
}

bool SubscriptionTestSuite::_verifyResponse(struct json_object* jobj, std::string key, int channel, std::string state)
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

    if(key.compare(json_object_get_string(keyJobj)) != 0)
    {
        json_object_put(jobj);
        return false;
    }

    if(!json_object_object_get_ex(messageJobj, "channel", &channelJobj))
    {
        json_object_put(jobj);
        return false;
    }

    int ch = json_object_get_int(channelJobj);

    if(channel > 0) 
    {
        if(channel != ch)
        {
            json_object_put(jobj);
            return false;
        }
    }

    if(!json_object_object_get_ex(messageJobj, "state", &stateJobj))
    {
        json_object_put(jobj);
        return false;
    }

    if(state.compare(json_object_get_string(stateJobj)) != 0)
    {
        json_object_put(jobj);
        return false;
    }

    json_object_put(jobj);
    return true;
}

bool SubscriptionTestSuite::_setPrecondition()
{
    if(SubscriptionTestSuite::_testDataPath.empty() == false)
    {
        if(SubscriptionTestSuite::_setUpAcceptedList()) //to test register client
            return true;
        else if(SubscriptionTestSuite::_setUpCreatedChannel()) //to test register channel
            return true;
        else if(SubscriptionTestSuite::_setUpRegisteredChannel()) //to test delete channel and client 
            return true;
        else
            return false;
    }

    return false;
}

bool SubscriptionTestSuite::_setUpRegisteredChannel()
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

    jobj = json_object_from_file(SubscriptionTestSuite::_testDataPath.c_str());

    if( (jobj == NULL) || is_error(jobj) )
    {
        std::cout << "Test Data is NOT JSON" << std::endl;
        std::cout << SubscriptionTestSuite::_testDataPath<< std::endl;
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

bool SubscriptionTestSuite::_setUpAcceptedList()
{
    struct json_object* jobj = NULL;
    struct json_object* listJobj = NULL;
    struct array_list* arrList = NULL;

    int arrSize = 0;
    int arrIndex = 0;

    std::string key;

    jobj = json_object_from_file(SubscriptionTestSuite::_testDataPath.c_str());

    if( (jobj == NULL) || is_error(jobj) )
        return false;

    if(!json_object_object_get_ex(jobj, "acceptedList", &listJobj))
    {
        json_object_put(jobj);
        return false;
    }

    arrList = json_object_get_array(listJobj);

    if(arrList == NULL)
    {
        json_object_put(jobj);
        return false;
    }

    arrSize = array_list_length(arrList);

    for(arrIndex = 0; arrIndex < arrSize; arrIndex++)
    {
        struct json_object* arrItemJobj = NULL;
        arrItemJobj = static_cast<json_object*>(array_list_get_idx(arrList, arrIndex));
        key.assign(json_object_get_string(arrItemJobj));
        mcHubd::ConnectionInfo::getInstance()->addAcceptedKey(key);
    }

    json_object_put(jobj);

    return true;
}

bool SubscriptionTestSuite::_setUpCreatedChannel()
{
    struct json_object* jobj = NULL;
    struct json_object* psListJobj = NULL;
    struct json_object* chListJobj = NULL;

    struct array_list* psArrList = NULL;
    struct array_list* chArrList = NULL;

    struct json_object* psNameJobj = NULL;
    struct json_object* pidJobj = NULL;
    struct json_object* keyJobj = NULL;

    int arrSize = 0;
    int arrIndex = 0;

    jobj = json_object_from_file(SubscriptionTestSuite::_testDataPath.c_str());

    if( (jobj == NULL) || is_error(jobj) )
    {
        std::cout << "Test Data is NOT JSON" << std::endl;
        std::cout << SubscriptionTestSuite::_testDataPath<< std::endl;
        return false;
    }

    if(!json_object_object_get_ex(jobj, "createdChannelList", &chListJobj))
    {
        json_object_put(jobj);
        return false;
    }

    chArrList = json_object_get_array(chListJobj);

    if(chArrList == NULL)
    {
        json_object_put(jobj);
        return false;
    }

    arrSize = array_list_length(chArrList);

    for(arrIndex = 0; arrIndex < arrSize; arrIndex++)
    {
        std::string key;
        struct json_object* chArrItemJobj = NULL;

        chArrItemJobj = static_cast<json_object*>(array_list_get_idx(chArrList, arrIndex));

        if(!json_object_object_get_ex(chArrItemJobj, "key", &keyJobj))
        {
            json_object_put(jobj);
            return false;
        }

        key.assign(json_object_get_string(keyJobj));
        mcHubd::TaskSet::getInstance()->addWaitToReadyTask(key);
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
        std::string psName;
        int pid;
        struct json_object* psArrItemJobj = NULL;

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

        psName.assign(json_object_get_string(psNameJobj));
        pid = json_object_get_int(pidJobj);

        mcHubd::ConnectionInfo::getInstance()->addConnectedClientKeyMap(psName, static_cast<pid_t>(pid));
    }

    json_object_put(jobj);
    return true;

}

