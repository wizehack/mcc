#include <iostream>
#include "registerChannelTestSuite.h"
#include "../registerChannelHandler.h"
#include "../registerClientHandler.h"
#include "../mcHubType.h"
#include "../message.h"
#include "../taskSet.h"
#include "../connectionInfo.h"
#include "../manager.h"
#include "../mediator.h"
#include "../channelStatusMediator.h"
#include "../channelManager.h"

std::string RegisterChannelTestSuite::_testDataPath("");

RegisterChannelTestSuite::RegisterChannelTestSuite() : TestSuite(){}
RegisterChannelTestSuite::~RegisterChannelTestSuite(){}

bool RegisterChannelTestSuite::request(TestOption* opt)
{
    if(opt == NULL)
        return false;

    if(opt->getCategory().compare("RegisterChannel") == 0)
    {
        RegisterChannelTestSuite::_testDataPath = opt->getTestDataPath();
        this->registerTestCase();
        return this->exec(opt->getTestCaseID());
    }

    if(this->m_next)
    {
        return this->m_next->request(opt);
    }

    return false;
}

void RegisterChannelTestSuite::registerTestCase()
{
    if(this->setPrecondition())
    {
        this->add(1, RegisterChannelTestSuite::_testRegisterCreatedChannel);
        this->add(2, RegisterChannelTestSuite::_testRegisterNOTCreatedChannel);
        this->add(3, RegisterChannelTestSuite::_testRegisterDuplicatedChannel);
    }
    else
    {
        std::cout << "Precondition Failed" << std::endl;
    }
}

bool RegisterChannelTestSuite::setPrecondition()
{
    struct json_object* jobj = NULL;
    struct json_object* psListJobj = NULL;
    struct json_object* chListJobj = NULL;

    struct array_list* psArrList = NULL;
    struct array_list* chArrList = NULL;

    struct json_object* psArrItemJobj = NULL;
    struct json_object* chArrItemJobj = NULL;
    struct json_object* psNameJobj = NULL;
    struct json_object* pidJobj = NULL;
    struct json_object* keyJobj = NULL;

    int arrSize = 0;
    int arrIndex = 0;

    jobj = json_object_from_file(RegisterChannelTestSuite::_testDataPath.c_str());

    if( (jobj == NULL) || is_error(jobj) )
    {
        std::cout << "Test Data is NOT JSON" << std::endl;
        std::cout << RegisterChannelTestSuite::_testDataPath<< std::endl;
        return false;
    }

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

    if(!json_object_object_get_ex(jobj, "createdChannelList", &chListJobj))
    {
        std::cout << "createdChannelList key is NOT found" << std::endl;
        json_object_put(jobj);
        return false;
    }

    chArrList = json_object_get_array(chListJobj);

    if(chArrList == NULL)
    {
        std::cout << "List is NULL" << std::endl;
        json_object_put(jobj);
        return false;
    }

    arrSize = array_list_length(chArrList);

    for(arrIndex = 0; arrIndex < arrSize; arrIndex++)
    {
        std::string key;

        chArrItemJobj = static_cast<json_object*>(array_list_get_idx(chArrList, arrIndex));

        if(!json_object_object_get_ex(chArrItemJobj, "key", &keyJobj))
        {
            std::cout << "key is NOT found" << std::endl;
            json_object_put(jobj);
            return false;
        }

        key.assign(json_object_get_string(keyJobj));
        mcHubd::TaskSet::getInstance()->addWaitToReadyTask(key);
    }

    json_object_put(jobj);
    return true;
}

bool RegisterChannelTestSuite::_testRegisterCreatedChannel()
{
    mcHubd::RegisterChannelHandler regChannelHandler;
    mcHubd::Mediator* mediator = new mcHubd::ChannelStatusMediator();
    mcHubd::Manager* mgr = new mcHubd::ChannelManager(mediator);

    std::shared_ptr<mcHubd::Message> msg = std::make_shared<mcHubd::Message>(mcHubd::REQ_REG_CHANNEL);
    std::string body;

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

    body.assign("{\"key\": \"com.mchannel.test.t2\", \"channel\": 1001}");
    msg->setBody(body);
    regChannelHandler.request(msg);

    body.assign("{\"key\": \"com.mchannel.test.t3\", \"channel\": 1002}");
    msg->setBody(body);
    regChannelHandler.request(msg);

    body.assign("{\"key\": \"com.mchannel.test.t4\", \"channel\": 1003}");
    msg->setBody(body);
    regChannelHandler.request(msg);

    body.assign("{\"key\": \"com.mchannel.test.t5\", \"channel\": 1004}");
    msg->setBody(body);
    regChannelHandler.request(msg);

    body.assign("{\"key\": \"com.mchannel.test.t6\", \"channel\": 1005}");
    msg->setBody(body);
    regChannelHandler.request(msg);

    body.assign("{\"key\": \"com.mchannel.test.t7\", \"channel\": 1006}");
    msg->setBody(body);
    regChannelHandler.request(msg);

    body.assign("{\"key\": \"com.mchannel.test.t8\", \"channel\": 1007}");
    msg->setBody(body);
    regChannelHandler.request(msg);

    body.assign("{\"key\": \"com.mchannel.test.t9\", \"channel\": 1008}");
    msg->setBody(body);
    regChannelHandler.request(msg);

    body.assign("{\"key\": \"com.mchannel.test.t10\", \"channel\": 1009}");
    msg->setBody(body);
    regChannelHandler.request(msg);

    if(mgr->isAvailable(t1) == false)
        return false;
    if(mgr->isAvailable(t2) == false)
        return false;
    if(mgr->isAvailable(t3) == false)
        return false;
    if(mgr->isAvailable(t4) == false)
        return false;
    if(mgr->isAvailable(t5) == false)
        return false;
    if(mgr->isAvailable(t6) == false)
        return false;
    if(mgr->isAvailable(t7) == false)
        return false;
    if(mgr->isAvailable(t8) == false)
        return false;
    if(mgr->isAvailable(t9) == false)
        return false;
    if(mgr->isAvailable(t10) == false)
        return false;

    return true;
}

bool RegisterChannelTestSuite::_testRegisterNOTCreatedChannel()
{
    mcHubd::RegisterChannelHandler regChannelHandler;
    mcHubd::Mediator* mediator = new mcHubd::ChannelStatusMediator();
    mcHubd::Manager* mgr = new mcHubd::ChannelManager(mediator);

    std::shared_ptr<mcHubd::Message> msg = std::make_shared<mcHubd::Message>(mcHubd::REQ_REG_CHANNEL);
    std::string body;

    /*Registered key*/
    std::string f1("com.mchannel.foo.f1");
    std::string f2("com.mchannel.foo.f2");

    /*NOT registered key*/
    std::string b1("com.mchannel.bar.b1");
    std::string b2("com.mchannel.bar.b2");

    body.assign("{\"key\": \"com.mchannel.foo.f1\", \"channel\": 2000}");
    msg->setBody(body);
    regChannelHandler.request(msg);

    body.assign("{\"key\": \"com.mchannel.foo.f2\", \"channel\": 2001}");
    msg->setBody(body);
    regChannelHandler.request(msg);

    if(mgr->isAvailable(b1))
        return false;
    if(mgr->isAvailable(b2))
        return false;

    return true;
}

bool RegisterChannelTestSuite::_testRegisterDuplicatedChannel()
{
    mcHubd::RegisterChannelHandler regChannelHandler;
    mcHubd::Mediator* mediator = new mcHubd::ChannelStatusMediator();
    mcHubd::Manager* mgr = new mcHubd::ChannelManager(mediator);
    std::shared_ptr<mcHubd::Message> msg = std::make_shared<mcHubd::Message>(mcHubd::REQ_REG_CHANNEL);
    std::string body;

    std::string f1("com.mchannel.foo.f1");
    std::map<std::string, key_t> availableMap;
    std::map<std::string, key_t>::iterator it;

    body.assign("{\"key\": \"com.mchannel.foo.f1\", \"channel\": 2000}");
    msg->setBody(body);
    regChannelHandler.request(msg);

    body.assign("{\"key\": \"com.mchannel.foo.f1\", \"channel\": 4000}");
    msg->setBody(body);
    regChannelHandler.request(msg);

    if(mgr->isAvailable(f1) == false)
        return false;

    availableMap = mcHubd::ConnectionInfo::getInstance()->getAvailableList();
    it = availableMap.find(f1);

    if(it->second == 4000)
        return false;

    return true;
}
