#include <iostream>
#include "registerChannelTestSuite.h"
#include "testData.h"
#include "reqMsgMaker.h"
#include "udpClient.h"

std::string RegisterChannelTestSuite::_testDataPath("");
std::string RegisterChannelTestSuite::_psName("");
std::list<std::string> RegisterChannelTestSuite::_keyList;

RegisterChannelTestSuite::RegisterChannelTestSuite() : TestSuite() {}
RegisterChannelTestSuite::~RegisterChannelTestSuite(){}

bool RegisterChannelTestSuite::request(TestOption* opt)
{
    std::cout << __PRETTY_FUNCTION__ << "[" << __LINE__ << "] " << std::endl;
    if(opt == NULL)
        return false;

    std::cout << __PRETTY_FUNCTION__ << "[" << __LINE__ << "] " << std::endl;
    if(opt->getCategory().compare("RegisterChannel") == 0)
    {
        std::cout << __PRETTY_FUNCTION__ << "[" << __LINE__ << "] " << std::endl;
        RegisterChannelTestSuite::_testDataPath = opt->getTestDataPath();
        this->registerTestCase();
        return this->exec(opt->getTestCaseID());
    }
    else if(this->m_next)
    {
        return this->m_next->request(opt);
    }

    std::cout << __PRETTY_FUNCTION__ << "[" << __LINE__ << "] " << std::endl;
    return false;
}

void RegisterChannelTestSuite::registerTestCase()
{
    if(RegisterChannelTestSuite::_setPrecondition())
    {
        this->add(1, RegisterChannelTestSuite::_test);
    }
    else
    {
        std::cout << "precondition error" << std::endl;
    }
}

bool RegisterChannelTestSuite::_setPrecondition()
{
    if(RegisterChannelTestSuite::_testDataPath.empty() == false)
    {
        TestData* td = new TestData(RegisterChannelTestSuite::_testDataPath);
        RegisterChannelTestSuite::_psName = td->getProcessName();
        RegisterChannelTestSuite::_keyList = td->getKeyList();
        delete td;
        return true;
    }

    return false;
}

bool RegisterChannelTestSuite::_test()
{
    ReqMsgMaker rmm;
    std::string message;
    std::string ip("127.0.0.1");
    int port = 6000; //default port
    int pid = getpid();

    std::cout << __PRETTY_FUNCTION__ << "[" << __LINE__ << "] " << std::endl;
    rmm.setPid(pid);
    rmm.setProcessName(RegisterChannelTestSuite::_psName);
    rmm.setChannelList(RegisterChannelTestSuite::_keyList);

    message = rmm.makeRegisterClientMsg();

    UDPClient* udp = new UDPClient(ip, port, MAX_BUF_SIZE);

    if(udp->conn())
    {
        std::string regCliresp = udp->send(message);
        std::string reqChResp;
        std::map<std::string, int> map = rmm.getKeyChannelMap(regCliresp);
        std::map<std::string, int>::iterator itor;

        for(itor = map.begin(); itor != map.end(); ++itor)
        {
            message.clear();
            reqChResp.clear();
            message = rmm.makeRegisterChannelMsg(itor->first, itor->second);
            reqChResp = udp->send(message);
            struct json_object* respJobj = json_tokener_parse(reqChResp.c_str());

            if(RegisterChannelTestSuite::_verifyResponseOk(respJobj) == false)
            {
                udp->quit();
                delete udp;
                return false;
            }

            json_object_put(respJobj);
        }

        udp->quit();
    }

    delete udp;
    return true;
}

bool RegisterChannelTestSuite::_verifyResponseOk(struct json_object* jobj)
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

    json_object_put(jobj);
    return true;
}
