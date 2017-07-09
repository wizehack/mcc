#include <iostream>
#include "deleteChannelTestSuite.h"
#include "testData.h"
#include "reqMsgMaker.h"
#include "udpClient.h"


std::string DeleteChannelTestSuite::_testDataPath("");
std::string DeleteChannelTestSuite::_psName("");
std::list<std::string> DeleteChannelTestSuite::_keyList;

DeleteChannelTestSuite::DeleteChannelTestSuite() : TestSuite() {}
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
    else if(this->m_next)
    {
        return this->m_next->request(opt);
    }

    return false;
}


void DeleteChannelTestSuite::registerTestCase()
{
    if(DeleteChannelTestSuite::_setPrecondition())
    {
        this->add(1, DeleteChannelTestSuite::_test);
    }
    else
    {
        std::cout << "Precondition Failed" << std::endl;
    }
}

bool DeleteChannelTestSuite::_setPrecondition()
{
    if(DeleteChannelTestSuite::_testDataPath.empty() == false)
    {
        TestData* td = new TestData(DeleteChannelTestSuite::_testDataPath);
        DeleteChannelTestSuite::_psName = td->getProcessName();
        DeleteChannelTestSuite::_keyList = td->getKeyList();
        delete td;
        return true;
    }

    return false;
}

bool DeleteChannelTestSuite::_test()
{
    ReqMsgMaker rmm;
    std::string message;
    std::string ip("127.0.0.1");
    int port = 6000; //default port
    int pid = getpid();

    rmm.setPid(pid);
    rmm.setProcessName(DeleteChannelTestSuite::_psName);
    rmm.setChannelList(DeleteChannelTestSuite::_keyList);

    message = rmm.makeRegisterClientMsg();

    UDPClient* udp = new UDPClient(ip, port, MAX_BUF_SIZE);

    if(udp->conn())
    {
        std::cout << __PRETTY_FUNCTION__ << "[" << __LINE__ << "] " << std::endl;
        std::string response = udp->send(message);
        std::cout << __PRETTY_FUNCTION__ << "[" << __LINE__ << "] response: " << response << std::endl;

        struct json_object* registerClientResponseJobj = json_tokener_parse(response.c_str());
        std::cout << __PRETTY_FUNCTION__ << "[" << __LINE__ << "] " << std::endl;

        std::string payload;

        if((registerClientResponseJobj == NULL) || is_error(registerClientResponseJobj))
        {
            udp->quit();
            return false;
        }
        else
        {
            payload.assign(json_object_get_string(registerClientResponseJobj));
        }

        std::map<std::string, int> map = rmm.getKeyChannelMap(payload);
        std::map<std::string, int>::iterator itor;

        json_object_put(registerClientResponseJobj);

        for(itor = map.begin(); itor != map.end(); ++itor)
        {
            message.clear();
            message = rmm.makeRegisterChannelMsg(itor->first, itor->second);
            udp->send(message);
        }

        for(itor = map.begin(); itor != map.end(); ++itor)
        {
            message.clear();
            message = rmm.makeDeleteChannelMsg(itor->first, itor->second);
            std::string resp = udp->send(message);

            std::cout << __PRETTY_FUNCTION__ << "[" << __LINE__ << "] resp: " << resp << std::endl;
            struct json_object* deleteChannelResponseJobj = json_tokener_parse(resp.c_str());

            if(DeleteChannelTestSuite::_verifyResponseOk(deleteChannelResponseJobj) == false)
            {
                udp->quit();
                return false;
            }

            json_object_put(deleteChannelResponseJobj);
        }

        udp->quit();
    }
    delete udp;
    return true;
}

bool DeleteChannelTestSuite::_verifyResponseOk(struct json_object* jobj)
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

    if(state.compare("closed") != 0)
        return false;

    if(!keyJobj || !channelJobj || !stateJobj)
    {
        json_object_put(jobj);
        return false;
    }

    json_object_put(jobj);
    return true;
}
