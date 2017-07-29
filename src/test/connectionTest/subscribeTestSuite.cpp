#include <iostream>
#include "subscribeTestSuite.h"
#include "testData.h"
#include "reqMsgMaker.h"
#include "udpClient.h"

int SubscribeTestSuite::_pid = 0;
std::string SubscribeTestSuite::_testDataPath("");
std::string SubscribeTestSuite::_psName("");
std::list<std::string> SubscribeTestSuite::_keyList;

SubscribeTestSuite::SubscribeTestSuite() : TestSuite(){}
SubscribeTestSuite::~SubscribeTestSuite(){}

bool SubscribeTestSuite::request(TestOption* opt)
{
    if(opt == NULL)
        return false;

    if(opt->getCategory().compare("Subscribe") == 0)
    {
        SubscribeTestSuite::_testDataPath = opt->getTestDataPath();
        this->registerTestCase();
        return this->exec(opt->getTestCaseID());
    }

    if(this->m_next)
    {
        return this->m_next->request(opt);
    }

    return false;
}

void SubscribeTestSuite::registerTestCase()
{
    if(SubscribeTestSuite::_setPrecondition())
    {
        this->add(1, SubscribeTestSuite::_test);
    }
    else
    {
        std::cout << "precondition error" << std::endl;
    }
}

bool SubscribeTestSuite::_setPrecondition()
{
    if(SubscribeTestSuite::_testDataPath.empty() == false)
    {
        TestData* td = new TestData(SubscribeTestSuite::_testDataPath);
        SubscribeTestSuite::_pid = td->getPID();
        SubscribeTestSuite::_psName = td->getProcessName();
        SubscribeTestSuite::_keyList = td->getKeyList();
        delete td;
        return true;
    }

    return false;
}

bool SubscribeTestSuite::_test()
{
    bool isSuccess = false;
    ReqMsgMaker rmm;
    std::string message;
    std::string ip("127.0.0.1");
    int port = 6000; //default port
    int pid = SubscribeTestSuite::_pid;
    const int TEST_CNT = 1;

    rmm.setPid(pid);
    rmm.setProcessName(SubscribeTestSuite::_psName);
    rmm.setChannelList(SubscribeTestSuite::_keyList);

    message = rmm.makeRegisterClientMsgWithSubscribe();
    std::cout << __PRETTY_FUNCTION__ << "[" << __LINE__ << "] request: " << message << std::endl;

    UDPClient* udp = new UDPClient(ip, port, MAX_BUF_SIZE);

    if(udp->conn())
    {
        std::string response = udp->send(message);
        struct json_object* jobj = json_tokener_parse(response.c_str());
        if(SubscribeTestSuite::_verifyResponseOk(jobj) == false)
        {
            std::cout << __PRETTY_FUNCTION__ << "[" << __LINE__ << "] " << "fail to register client" << std::endl;
            udp->quit();
            return false;
        }

        isSuccess = true;
    }

    for(int i=0; i<TEST_CNT; ++i)
    {
        std::string notice = udp->getSubscribe();
        std::cout << __PRETTY_FUNCTION__ << "[" << __LINE__ << "] " << i << ": " << notice << std::endl;

        struct json_object* jobj = json_tokener_parse(notice.c_str());

        if(SubscribeTestSuite::_verify(jobj) == false)
        {
            isSuccess = false;
            break;
        }

        if(jobj)
            json_object_put(jobj);
    }

    delete udp;
    return isSuccess;
}

bool SubscribeTestSuite::_verify(struct json_object* jobj)
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

    json_object_put(jobj);
    return true;
}

bool SubscribeTestSuite::_verifyResponseOk(struct json_object* jobj)
{
    struct json_object* codeJobj = NULL;
    struct json_object* retJobj = NULL;
    struct json_object* messageJobj = NULL;
    struct array_list* keyChannelList = NULL;
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

            keyChannelJobj = static_cast<json_object*>(array_list_get_idx(keyChannelList, arrIndex));

            if(!json_object_object_get_ex(keyChannelJobj, "key", &keyJobj))
            {
                json_object_put(jobj);
                return false;
            }

            if(!json_object_object_get_ex(keyChannelJobj, "channel", &channelJobj))
            {
                json_object_put(jobj);
                return false;
            }

            if(!keyJobj || !channelJobj)
            {
                json_object_put(jobj);
                return false;
            }
        }
    }
    else
    {
        json_object_put(jobj);
        return false;
    }

    return true;
}
