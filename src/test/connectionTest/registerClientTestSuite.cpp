#include <iostream>
#include "registerClientTestSuite.h"
#include "testData.h"
#include "reqMsgMaker.h"
#include "udpClient.h"

int RegisterClientTestSuite::_pid = 0;
std::string RegisterClientTestSuite::_testDataPath("");
std::string RegisterClientTestSuite::_psName("");
std::list<std::string> RegisterClientTestSuite::_keyList;

RegisterClientTestSuite::RegisterClientTestSuite() : TestSuite(){}
RegisterClientTestSuite::~RegisterClientTestSuite(){}

bool RegisterClientTestSuite::request(TestOption* opt)
{
    if(opt == NULL)
        return false;

    if(opt->getCategory().compare("RegisterClient") == 0)
    {
        RegisterClientTestSuite::_testDataPath = opt->getTestDataPath();
        this->registerTestCase();
        return this->exec(opt->getTestCaseID());
    }

    if(this->m_next)
    {
        return this->m_next->request(opt);
    }

    return false;
}

void RegisterClientTestSuite::registerTestCase()
{
    if(RegisterClientTestSuite::_setPrecondition())
    {
        this->add(1, RegisterClientTestSuite::_test);
    }
    else
    {
        std::cout << "precondition error" << std::endl;
    }
}

bool RegisterClientTestSuite::_setPrecondition()
{
    if(RegisterClientTestSuite::_testDataPath.empty() == false)
    {
        TestData* td = new TestData(RegisterClientTestSuite::_testDataPath);
        RegisterClientTestSuite::_pid = td->getPID();
        RegisterClientTestSuite::_psName = td->getProcessName();
        RegisterClientTestSuite::_keyList = td->getKeyList();
        delete td;
        return true;
    }

    return false;
}

bool RegisterClientTestSuite::_test()
{
    bool isSuccess = false;
    ReqMsgMaker rmm;
    std::string message;
    std::string ip("127.0.0.1");
    int port = 6000; //default port
    int pid = RegisterClientTestSuite::_pid;

    rmm.setPid(pid);
    rmm.setProcessName(RegisterClientTestSuite::_psName);
    rmm.setChannelList(RegisterClientTestSuite::_keyList);

    message = rmm.makeRegisterClientMsg();

    UDPClient* udp = new UDPClient(ip, port, MAX_BUF_SIZE);

    if(udp->conn())
    {
        std::string response = udp->send(message);
        struct json_object* jobj = json_tokener_parse(response.c_str());
        if(RegisterClientTestSuite::_verifyResponseOk(jobj))
            isSuccess = true;
        udp->quit();
    }

    delete udp;
    return isSuccess;
}

bool RegisterClientTestSuite::_verifyResponseOk(struct json_object* jobj)
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
