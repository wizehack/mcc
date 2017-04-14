#include <iostream>
#include <chrono>
#include <thread>

#include "messageQueueTestSuite.h"
#include "registerClientTestSuite.h"
#include "registerChannelTestSuite.h"
#include "deleteChannelTestSuite.h"
#include "deleteClientTestSuite.h"
#include "messageQueueTestSuite.h"

#include "mcHubType.h"
#include "feature/message.h"
#include "feature/manager.h"
#include "feature/clientManager.h"
#include "feature/mediator.h"
#include "feature/channelStatusMediator.h"
#include "feature/testStub.h"
#include "feature/messageQueue.h"

int MessageQueueTestSuite::_respIndex = 0;
std::string MessageQueueTestSuite::_testDataPath("");

MessageQueueTestSuite::MessageQueueTestSuite(){}
MessageQueueTestSuite::~MessageQueueTestSuite(){}

bool MessageQueueTestSuite::request(TestOption* opt)
{
    if(opt == NULL)
        return false;

    if(opt->getCategory().compare("MessageQueue") == 0)
    {
        MessageQueueTestSuite::_testDataPath = opt->getTestDataPath();
        this->registerTestCase();
        return this->exec(opt->getTestCaseID());
    }

    if(this->m_next)
    {
        return this->m_next->request(opt);
    }

    return false;

}

bool MessageQueueTestSuite::_testOKResponse()
{
    bool isPassed = true;
    struct json_object* jobj = NULL;
    std::string response;

    /* Register client */
    std::shared_ptr<mcHubd::Message> sptrMsg = std::make_shared<mcHubd::Message>(mcHubd::REQ_REG_CLIENT);
    mcHubd::Message* msg = sptrMsg.get();
    std::string body;
    body.assign("{\"pid\": 1212, \"psName\": \"test\", \"keyList\":");
    body.append("[\"com.mchannel.test.t1\", \"com.mchannel.test.t2\"]");
    body.append("}");

    if(msg)
    {
        msg->setBody(body);
        g_async_queue_push(mcHubd::MessageQueue::getInstance()->getReqMessageQueue(), static_cast<gpointer>(msg));

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        response = mcHubd::TestStub::getInstance()->getRespMsg(MessageQueueTestSuite::_respIndex);
        MessageQueueTestSuite::_respIndex++;

        jobj = json_tokener_parse(response.c_str());

        if((jobj == NULL) || is_error(jobj))
            return false;

        if(RegisterClientTestSuite::_verifyResponseOk(jobj) == false)
        {
            json_object_put(jobj);
            return false;
        }

        json_object_put(jobj);
    }
    else
    {
        std::cout << __LINE__ << ": precondition failed" << std::endl;
        return false;
    }

    /* Register channel */
    if(MessageQueueTestSuite::_checkRequestType(response, mcHubd::REQ_REG_CHANNEL) == false)
        return false;

    /* Delete channel */
    if(MessageQueueTestSuite::_checkRequestType(response, mcHubd::REQ_DEL_CHANNEL) == false)
        return false;

    /* Delete Client */
    body.clear();

    sptrMsg = std::make_shared<mcHubd::Message>(mcHubd::REQ_DEL_CLIENT);
    msg = sptrMsg.get();

    if(msg)
    {
        body.assign("{\"pid\": 1212, \"psName\": \"test\"}");
        msg->setBody(body);

        g_async_queue_push(mcHubd::MessageQueue::getInstance()->getReqMessageQueue(), static_cast<gpointer>(msg));

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        jobj = json_tokener_parse(mcHubd::TestStub::getInstance()->getRespMsg(MessageQueueTestSuite::_respIndex).c_str());

        if((jobj == NULL) || is_error(jobj))
            isPassed = false;

        if(DeleteClientTestSuite::_verifyResponseOk(jobj) == false)
            isPassed = false;

        MessageQueueTestSuite::_respIndex++;
        json_object_put(jobj);
    }
    else
    {
        std::cout << __LINE__ << ": precondition failed" << std::endl;
        isPassed = false;
    }

    return isPassed;
}

void MessageQueueTestSuite::registerTestCase()
{
    mcHubd::Mediator* mediator = new mcHubd::ChannelStatusMediator();
    mcHubd::Manager* manager = new mcHubd::ClientManager(mediator);
    manager->_setUpAcceptedList(MessageQueueTestSuite::_testDataPath);

    g_thread_new("msg processor", mcHubd::MessageQueue::_msgProcessor, NULL);

    this->add(1, MessageQueueTestSuite::_testOKResponse);

    delete mediator;
    delete manager;
}

std::map<std::string, key_t> MessageQueueTestSuite::_getkeyChannelMap(std::string response)
{
    std::map<std::string, key_t> map;
    struct json_object* jobj = NULL;
    struct array_list* keyChannelList = NULL;
    struct json_object* codeJobj = NULL;
    struct json_object* messageJobj = NULL;
    struct json_object* returnJobj = NULL;

    jobj = json_tokener_parse(response.c_str());

    if((jobj == NULL) || is_error(jobj))
        return map;

    if(!json_object_object_get_ex(jobj, "code", &codeJobj))
    {
        json_object_put(jobj);
        return map;
    }

    if(json_object_get_int(codeJobj) != 0)
    {
        json_object_put(jobj);
        return map;
    }

    if(!json_object_object_get_ex(jobj, "return", &returnJobj))
    {
        json_object_put(jobj);
        return map;
    }

    if(json_object_get_boolean(returnJobj) == false)
    {
        json_object_put(jobj);
        return map;
    }

    if(!json_object_object_get_ex(jobj, "message", &messageJobj))
    {
        json_object_put(jobj);
        return map;
    }

    keyChannelList = json_object_get_array(messageJobj);

    if(keyChannelList)
    {
        int arrSize = array_list_length(keyChannelList);
        int arrIndex = 0;

        for(arrIndex = 0; arrIndex < arrSize; arrIndex++)
        {
            struct json_object* keyChannelJobj = NULL;
            struct json_object* keyJobj = NULL;
            struct json_object* channelJobj = NULL;

            std::string key;

            keyChannelJobj = static_cast<json_object*>(array_list_get_idx(keyChannelList, arrIndex));

            if(!json_object_object_get_ex(keyChannelJobj, "key", &keyJobj))
            {
                json_object_put(jobj);
                return map;
            }

            if(!json_object_object_get_ex(keyChannelJobj, "channel", &channelJobj))
            {
                json_object_put(jobj);
                return map;
            }

            if(keyJobj && channelJobj)
            {
                int channel;
                key.assign(json_object_get_string(keyJobj));
                channel = static_cast<key_t>(json_object_get_int(channelJobj));
                map.insert(std::pair<std::string, key_t>(key, channel));
            }
        }
    }

    json_object_put(jobj);
    return map;
}


bool MessageQueueTestSuite::_checkRequestType(std::string keyChannelResponse, mcHubd::MSGTYPE type)
{
    std::shared_ptr<mcHubd::Message> sptrMsg;

    std::map<std::string, key_t> keyChannelMap;
    std::map<std::string, pid_t>::iterator itor;

    keyChannelMap = MessageQueueTestSuite::_getkeyChannelMap(keyChannelResponse);
    itor = keyChannelMap.begin();

    while(itor != keyChannelMap.end())
    {
        mcHubd::Message* msg = NULL;
        bool isPassed = true;
        struct json_object* respJobj = NULL;
        std::string body;

        sptrMsg = std::make_shared<mcHubd::Message>(type);
        msg = sptrMsg.get();
        body = "{\"key\": \"" + itor->first + "\", \"channel\":" + std::to_string(itor->second) + "})";

        msg->setBody(body);
        g_async_queue_push(mcHubd::MessageQueue::getInstance()->getReqMessageQueue(), static_cast<gpointer>(msg));
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        respJobj = json_tokener_parse(mcHubd::TestStub::getInstance()->getRespMsg(MessageQueueTestSuite::_respIndex).c_str());

        if(type == mcHubd::REQ_REG_CHANNEL)
            isPassed = RegisterChannelTestSuite::_verifyResponseOk(respJobj);

        if(type == mcHubd::REQ_DEL_CHANNEL)
            isPassed = DeleteChannelTestSuite::_verifyResponseOk(respJobj);

        MessageQueueTestSuite::_respIndex++;

        ++itor;

        json_object_put(respJobj);

        if(isPassed == false)
            return false;
    }

    return true;
}
