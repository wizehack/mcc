#include <iostream>
#include "deleteChannelTestSuite.h"
#include "../mcHubType.h"
#include "../connectionInfo.h"
#include "../taskSet.h"
#include "../message.h"
#include "../manager.h"
#include "../mediator.h"
#include "../channelStatusMediator.h"
#include "../channelManager.h"
#include "../deleteChannelHandler.h"


std::string DeleteChannelTestSuite::_testDataPath("");

DeleteChannelTestSuite::DeleteChannelTestSuite() :
    TestSuite(),
    m_testCaseID(0){}
DeleteChannelTestSuite::~DeleteChannelTestSuite(){}

bool DeleteChannelTestSuite::request(TestOption* opt)
{
    if(opt == NULL)
        return false;

    if(opt->getCategory().compare("DeleteChannel") == 0)
    {
        DeleteChannelTestSuite::_testDataPath = opt->getTestDataPath();
        this->registerTestCase();
        return this->exec(opt->getTestCaseID());
    }

    if(this->m_next)
    {
        return this->m_next->request(opt);
    }

    return false;
}


void DeleteChannelTestSuite::registerTestCase()
{
    if(this->setPrecondition())
    {
        this->add(1, DeleteChannelTestSuite::_testDeleteAvailableKey);
        this->add(2, DeleteChannelTestSuite::_testDeleteReadyKey);
        this->add(3, DeleteChannelTestSuite::_testDeleteEmptyChannel);
        this->add(4, DeleteChannelTestSuite::_testDeleteUnknownChannel);
    }
    else
    {
        std::cout << "Precondition Failed" << std::endl;
    }

}

bool DeleteChannelTestSuite::setPrecondition()
{
    struct json_object* jobj = NULL;
    bool isParsed;

    jobj = json_object_from_file(DeleteChannelTestSuite::_testDataPath.c_str());

    if( (jobj == NULL) || is_error(jobj) )
    {
        std::cout << "Test Data is NOT JSON" << std::endl;
        std::cout << DeleteChannelTestSuite::_testDataPath<< std::endl;
        return false;
    }

    isParsed = parseProcessList(jobj);
    isParsed = isParsed && parseConnectedKeyList(jobj);
    isParsed = isParsed && parseRegisteredKeyList(jobj);

    json_object_put(jobj);

    return isParsed;
}

bool DeleteChannelTestSuite::parseProcessList(struct json_object* jobj)
{
    struct json_object* psListJobj = NULL;
    struct array_list* psArrList = NULL;
    struct json_object* psArrItemJobj = NULL;
    struct json_object* psNameJobj = NULL;
    struct json_object* pidJobj = NULL;

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
        std::string psName;
        int pid;

        psArrItemJobj = static_cast<json_object*>(array_list_get_idx(psArrList, arrIndex));

        if(!json_object_object_get_ex(psArrItemJobj, "psname", &psNameJobj))
        {
            std::cout << "key is NOT found" << std::endl;
            json_object_put(jobj);
            return false;
        }

        if(!json_object_object_get_ex(psArrItemJobj, "pid", &pidJobj))
        {
            std::cout << "channel is NOT found" << std::endl;
            json_object_put(jobj);
            return false;
        }

        psName.assign(json_object_get_string(psNameJobj));
        pid = json_object_get_int(pidJobj);

        mcHubd::ConnectionInfo::getInstance()->addConnectedClientKeyMap(psName, static_cast<pid_t>(pid));
    }

    return true;
}

bool DeleteChannelTestSuite::parseConnectedKeyList(struct json_object* jobj)
{
    struct json_object* chListJobj = NULL;
    struct array_list* connList = NULL;
    struct json_object* connItemJobj = NULL;
    struct json_object* keyJobj = NULL;

    int arrSize = 0;
    int arrIndex = 0;

    if(!json_object_object_get_ex(jobj, "createdKeyList", &chListJobj))
    {
        std::cout << "createdChannelList key is NOT found" << std::endl;
        json_object_put(jobj);
        return false;
    }

    connList = json_object_get_array(chListJobj);

    if(connList == NULL)
    {
        std::cout << "List is NULL" << std::endl;
        json_object_put(jobj);
        return false;
    }

    arrSize = array_list_length(connList);

    for(arrIndex = 0; arrIndex < arrSize; arrIndex++)
    {
        std::string key;

        connItemJobj = static_cast<json_object*>(array_list_get_idx(connList, arrIndex));

        if(!json_object_object_get_ex(connItemJobj, "key", &keyJobj))
        {
            std::cout << "key is NOT found" << std::endl;
            json_object_put(jobj);
            return false;
        }

        key.assign(json_object_get_string(keyJobj));
        mcHubd::TaskSet::getInstance()->addWaitToReadyTask(key);
    }

    return true;
}

bool DeleteChannelTestSuite::parseRegisteredKeyList(struct json_object* jobj)
{
    struct json_object* regListJobj = NULL;
    struct array_list* regList = NULL;
    struct json_object* regItemJobj = NULL;
    struct json_object* keyJobj = NULL;
    struct json_object* channelJobj = NULL;

    int arrSize = 0;
    int arrIndex = 0;

    if(!json_object_object_get_ex(jobj, "registeredKeyList", &regListJobj))
    {
        std::cout << "createdChannelList key is NOT found" << std::endl;
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

bool DeleteChannelTestSuite::_testDeleteAvailableKey()
{
    mcHubd::DeleteChannelHandler handler;
    mcHubd::Mediator* mediator = new mcHubd::ChannelStatusMediator();
    mcHubd::Manager* mgr = new mcHubd::ChannelManager(mediator);

    std::shared_ptr<mcHubd::Message> msg = std::make_shared<mcHubd::Message>(mcHubd::REQ_DEL_CHANNEL);
    std::string body;
    std::string f1("com.mchannel.foo.f1");
    std::string f2("com.mchannel.foo.f2");

    body.assign("{\"key\": \"com.mchannel.foo.f1\", \"channel\": 2000}");
    msg->setBody(body);
    handler.request(msg);

    body.assign("{\"key\": \"com.mchannel.foo.f2\", \"channel\": 2001}");
    msg->setBody(body);
    handler.request(msg);

    if(mgr->isAvailable(f1))
        return false;
    if(mgr->isAvailable(f2))
        return false;
    if(mcHubd::TaskSet::getInstance()->isWaitingTask(f1))
       return false;
    if(mcHubd::TaskSet::getInstance()->isWaitingTask(f2))
       return false;

    return true;
}

bool DeleteChannelTestSuite::_testDeleteReadyKey()
{
    mcHubd::DeleteChannelHandler handler;
    mcHubd::Mediator* mediator = new mcHubd::ChannelStatusMediator();
    mcHubd::Manager* mgr = new mcHubd::ChannelManager(mediator);

    std::shared_ptr<mcHubd::Message> msg = std::make_shared<mcHubd::Message>(mcHubd::REQ_DEL_CHANNEL);
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

    body.assign("{\"key\": \"com.mchannel.test.t1\"}");
    msg->setBody(body);
    handler.request(msg);

    body.assign("{\"key\": \"com.mchannel.test.t2\"}");
    msg->setBody(body);
    handler.request(msg);

    body.assign("{\"key\": \"com.mchannel.foo.f1\"}");
    msg->setBody(body);
    handler.request(msg);

    body.assign("{\"key\": \"com.mchannel.foo.f2\"}");
    msg->setBody(body);
    handler.request(msg);

    body.assign("{\"key\": \"com.mchannel.bar.b1\"}");
    msg->setBody(body);
    handler.request(msg);

    body.assign("{\"key\": \"com.mchannel.bar.b2\"}");
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

    return true;
}

bool DeleteChannelTestSuite::_testDeleteEmptyChannel()
{
    mcHubd::DeleteChannelHandler handler;
    mcHubd::Mediator* mediator = new mcHubd::ChannelStatusMediator();
    mcHubd::Manager* mgr = new mcHubd::ChannelManager(mediator);

    std::shared_ptr<mcHubd::Message> msg = std::make_shared<mcHubd::Message>(mcHubd::REQ_DEL_CHANNEL);
    std::string body;
    std::string f1("com.mchannel.foo.f1");
    std::string f2("com.mchannel.foo.f2");

    body.assign("{\"key\": \"com.mchannel.foo.f1\"}");
    msg->setBody(body);
    handler.request(msg);

    body.assign("{\"key\": \"com.mchannel.foo.f2\"}");
    msg->setBody(body);
    handler.request(msg);

    if(mgr->isAvailable(f1) == false)
        return false;
    if(mgr->isAvailable(f2) == false)
        return false;
    if(mcHubd::TaskSet::getInstance()->isWaitingTask(f1))
       return false;
    if(mcHubd::TaskSet::getInstance()->isWaitingTask(f2))
       return false;

    return true;
}

bool DeleteChannelTestSuite::_testDeleteUnknownChannel()
{
    mcHubd::DeleteChannelHandler handler;
    mcHubd::Mediator* mediator = new mcHubd::ChannelStatusMediator();
    mcHubd::Manager* mgr = new mcHubd::ChannelManager(mediator);

    std::shared_ptr<mcHubd::Message> msg = std::make_shared<mcHubd::Message>(mcHubd::REQ_DEL_CHANNEL);
    std::string body;
    std::string f1("com.mchannel.foo.f1");
    std::string f2("com.mchannel.foo.f2");

    body.assign("{\"key\": \"com.mchannel.foo.f1\", \"channel\": 3000}");
    msg->setBody(body);
    handler.request(msg);

    body.assign("{\"key\": \"com.mchannel.foo.f2\", \"channel\": 3001}");
    msg->setBody(body);
    handler.request(msg);

    if(mgr->isAvailable(f1) == false)
        return false;
    if(mgr->isAvailable(f2) == false)
        return false;
    if(mcHubd::TaskSet::getInstance()->isWaitingTask(f1))
       return false;
    if(mcHubd::TaskSet::getInstance()->isWaitingTask(f2))
       return false;

    return true;
}
