#include <iostream>
#include "deleteClientTestSuite.h"
#include "../mcHubType.h"
#include "../connectionInfo.h"
#include "../taskSet.h"
#include "../message.h"
#include "../manager.h"
#include "../mediator.h"
#include "../channelStatusMediator.h"
#include "../channelManager.h"
#include "../deleteClientHandler.h"

std::string DeleteClientTestSuite::_testDataPath("");

DeleteClientTestSuite::DeleteClientTestSuite() :
    TestSuite(),
    m_testCaseID(0){}
DeleteClientTestSuite::~DeleteClientTestSuite(){}

bool DeleteClientTestSuite::request(TestOption* opt)
{
    if(opt == NULL)
        return false;

    if(opt->getCategory().compare("DeleteClient") == 0)
    {
        DeleteClientTestSuite::_testDataPath = opt->getTestDataPath();
        this->registerTestCase();
        return this->exec(opt->getTestCaseID());
    }

    if(this->m_next)
    {
        return this->m_next->request(opt);
    }

    return false;
}


void DeleteClientTestSuite::registerTestCase()
{
    if(DeleteClientTestSuite::_setPrecondition())
    {
        this->add(1, DeleteClientTestSuite::_testDeleteClient);
    }
    else
    {
        std::cout << "Precondition Failed" << std::endl;
    }
}

bool DeleteClientTestSuite::_setPrecondition()
{
    struct json_object* jobj = NULL;
    bool isParsed;

    jobj = json_object_from_file(DeleteClientTestSuite::_testDataPath.c_str());

    if( (jobj == NULL) || is_error(jobj) )
    {
        std::cout << "Test Data is NOT JSON" << std::endl;
        std::cout << DeleteClientTestSuite::_testDataPath<< std::endl;
        return false;
    }

    isParsed = DeleteClientTestSuite::_parseProcessList(jobj);
    isParsed = isParsed && DeleteClientTestSuite::_parseRegisteredKeyList(jobj);

    json_object_put(jobj);

    return isParsed;
}

bool DeleteClientTestSuite::_parseProcessList(struct json_object* jobj)
{
    struct json_object* psListJobj = NULL;
    struct array_list* psArrList = NULL;
    struct json_object* psNameJobj = NULL;
    struct json_object* pidJobj = NULL;
    struct json_object* keyListJobj = NULL;
    struct json_object* keyJobj = NULL;

    int arrSize = 0;
    int arrIndex = 0;

    if(!json_object_object_get_ex(jobj, "connectedProcessList", &psListJobj))
    {
        std::cout << "connectedProcessList key is NOT found" << std::endl;
        json_object_put(jobj);
        return false;
    }

    psArrList = json_object_get_array(psListJobj);

    if(psArrList == NULL)
    {
        std::cout << "List is NULL" << std::endl;
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
            std::cout << "psname is NOT found" << std::endl;
            json_object_put(jobj);
            return false;
        }

        if(!json_object_object_get_ex(psArrItemJobj, "pid", &pidJobj))
        {
            std::cout << "pid is NOT found" << std::endl;
            json_object_put(jobj);
            return false;
        }

        pid = json_object_get_int(pidJobj);

        if(!json_object_object_get_ex(psArrItemJobj, "keyList", &keyListJobj))
        {
            std::cout << "keyLIst is NOT found" << std::endl;
            json_object_put(jobj);
            return false;
        }

        keyArrList = json_object_get_array(keyListJobj);

        if(keyArrList == NULL)
        {
            std::cout << "keyList is NULL" << std::endl;
            json_object_put(jobj);
            return false;
        }

        keyArrSize = array_list_length(keyArrList);

        for(keyArrIndex = 0; keyArrIndex < keyArrSize; keyArrIndex++)
        {
            std::string clientKey;
            keyJobj = static_cast<json_object*>(array_list_get_idx(keyArrList, keyArrIndex));
            clientKey.assign(json_object_get_string(keyJobj));
            mcHubd::ConnectionInfo::getInstance()->addConnectedClientKeyMap(clientKey, static_cast<pid_t>(pid));
            mcHubd::TaskSet::getInstance()->addWaitToReadyTask(clientKey);
        }
    }

    return true;
}

bool DeleteClientTestSuite::_parseRegisteredKeyList(struct json_object* jobj)
{
    struct json_object* regListJobj = NULL;
    struct array_list* regList = NULL;
    struct json_object* keyJobj = NULL;
    struct json_object* channelJobj = NULL;

    int arrSize = 0;
    int arrIndex = 0;

    if(!json_object_object_get_ex(jobj, "registeredKeyList", &regListJobj))
    {
        std::cout << "createdClientList key is NOT found" << std::endl;
        json_object_put(jobj);
        return false;
    }

    regList = json_object_get_array(regListJobj);

    if(regList == NULL)
    {
        std::cout << "List is NULL" << std::endl;
        json_object_put(jobj);
        return false;
    }

    arrSize = array_list_length(regList);

    for(arrIndex = 0; arrIndex < arrSize; arrIndex++)
    {
        std::string key;
        key_t channel;
        struct json_object* regItemJobj = NULL;

        regItemJobj = static_cast<json_object*>(array_list_get_idx(regList, arrIndex));

        if(!json_object_object_get_ex(regItemJobj, "key", &keyJobj))
        {
            std::cout << "key is NOT found" << std::endl;
            json_object_put(jobj);
            return false;
        }

        if(!json_object_object_get_ex(regItemJobj, "channel", &channelJobj))
        {
            std::cout << "key is NOT found" << std::endl;
            json_object_put(jobj);
            return false;
        }

        key.assign(json_object_get_string(keyJobj));
        channel = json_object_get_int(channelJobj);

        mcHubd::ConnectionInfo::getInstance()->addAvailableKey(key, channel);
        mcHubd::TaskSet::getInstance()->rmWaitToReady(key);
    }

    return true;
}

bool DeleteClientTestSuite::_testDeleteClient()
{
    std::vector<std::string> clientMap;
    mcHubd::DeleteClientHandler handler;
    mcHubd::Mediator* mediator = new mcHubd::ChannelStatusMediator();
    mcHubd::Manager* mgr = new mcHubd::ChannelManager(mediator);

    std::shared_ptr<mcHubd::Message> msg = std::make_shared<mcHubd::Message>(mcHubd::REQ_DEL_CLIENT);

    std::string body;
    std::string t1("com.mchannel.test.t1");
    std::string t2("com.mchannel.test.t2");
    std::string f1("com.mchannel.foo.f1");
    std::string f2("com.mchannel.foo.f2");
    std::string b1("com.mchannel.bar.b1");
    std::string b2("com.mchannel.bar.b2");

    /* Check Precondition */
    if(mcHubd::TaskSet::getInstance()->isWaitingTask(t1) == false)
       return false;
    if(mcHubd::TaskSet::getInstance()->isWaitingTask(t2) == false)
       return false;
    if(mgr->isAvailable(f1) == false)
        return false;
    if(mgr->isAvailable(f2) == false)
        return false;
    if(mcHubd::TaskSet::getInstance()->isWaitingTask(b1) == false)
       return false;
    if(mcHubd::TaskSet::getInstance()->isWaitingTask(b2) == false)
       return false;

    body.assign("{\"pid\": 100, \"psName\": \"test\"}");
    msg->setBody(body);
    handler.request(msg);

    body.assign("{\"pid\": 200, \"psName\": \"foo\"}");
    msg->setBody(body);
    handler.request(msg);

    body.assign("{\"pid\": 400, \"psName\": \"bar\"}");
    msg->setBody(body);
    handler.request(msg);

    if(mcHubd::TaskSet::getInstance()->isWaitingTask(t1))
       return false;
    if(mcHubd::TaskSet::getInstance()->isWaitingTask(t2))
       return false;
    if(mcHubd::TaskSet::getInstance()->isWaitingTask(f1))
       return false;
    if(mcHubd::TaskSet::getInstance()->isWaitingTask(f2))
       return false;
    if(mcHubd::TaskSet::getInstance()->isWaitingTask(b1))
       return false;
    if(mcHubd::TaskSet::getInstance()->isWaitingTask(b2))
       return false;
    if(mgr->isAvailable(t1))
        return false;
    if(mgr->isAvailable(t2))
        return false;
    if(mgr->isAvailable(f1))
        return false;
    if(mgr->isAvailable(f2))
        return false;
    if(mgr->isAvailable(b1))
        return false;
    if(mgr->isAvailable(b2))
        return false;

    clientMap = mcHubd::ConnectionInfo::getInstance()->getConnectedClientKey(100);
    if(clientMap.empty() == false)
        return false;

    clientMap = mcHubd::ConnectionInfo::getInstance()->getConnectedClientKey(200);
    if(clientMap.empty() == false)
        return false;

    clientMap = mcHubd::ConnectionInfo::getInstance()->getConnectedClientKey(400);
    if(clientMap.empty() == false)
        return false;

    return true;
}
