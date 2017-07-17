#include <iostream>
#include "registerClientTestSuite.h"
#include "registerChannelTestSuite.h"
#include "requestChannelTestSuite.h"
#include "requestChannelTestData.h"
#include "reqMsgMaker.h"
#include "udpClient.h"

int RequestChannelTestSuite::_pid = 0;
int RequestChannelTestSuite::_tarPid = 0;
std::string RequestChannelTestSuite::_testDataPath("");
std::string RequestChannelTestSuite::_psName("");
std::string RequestChannelTestSuite::_targetPsName("");
std::list<std::string> RequestChannelTestSuite::_keyList;
std::list<std::string> RequestChannelTestSuite::_targetKeyList;

RequestChannelTestSuite::RequestChannelTestSuite() : TestSuite(){}
RequestChannelTestSuite::~RequestChannelTestSuite(){}

bool RequestChannelTestSuite::request(TestOption* opt)
{
    if(opt == NULL)
        return false;

    if(opt->getCategory().compare("RequestChannel") == 0)
    {
        RequestChannelTestSuite::_testDataPath = opt->getTestDataPath();
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
    if(RequestChannelTestSuite::_setPrecondition())
    {
        this->add(1, RequestChannelTestSuite::_test);
    }
    else
    {
        std::cout << "Precondition Failed" << std::endl;
    }
}

bool RequestChannelTestSuite::_setPrecondition()
{
    if(RequestChannelTestSuite::_testDataPath.empty() == false)
    {
        RequestChannelTestData* td = NULL;

        td = new RequestChannelTestData(RequestChannelTestSuite::_testDataPath);

        if(td)
        {
            ReqMsgMaker rmm;
            std::string message;
            std::string ip("127.0.0.1");
            int port = 6000; //default port

            RequestChannelTestSuite::_pid = td->getPID();
            RequestChannelTestSuite::_tarPid = td->getTargetPID();
            RequestChannelTestSuite::_psName = td->getProcessName();
            RequestChannelTestSuite::_keyList = td->getKeyList();
            RequestChannelTestSuite::_targetPsName = td->getTargetProcessName();
            RequestChannelTestSuite::_targetKeyList = td->getTargetKeyList();

            rmm.setPid(RequestChannelTestSuite::_tarPid);
            rmm.setProcessName(RequestChannelTestSuite::_targetPsName);
            rmm.setChannelList(RequestChannelTestSuite::_targetKeyList);

            message = rmm.makeRegisterClientMsg();
            std::cout << __PRETTY_FUNCTION__ << "[" << __LINE__ << "] req register client: " << message << std::endl;

            UDPClient* udp = new UDPClient(ip, port, MAX_BUF_SIZE);

            if(udp->conn())
            {
                std::string regCliresp = udp->send(message);;
                std::map<std::string, int> map = rmm.getKeyChannelMap(regCliresp);
                std::map<std::string, int>::iterator itor;

                std::cout << __PRETTY_FUNCTION__ << "[" << __LINE__ << "] res register client: " << regCliresp << std::endl;
                for(itor = map.begin(); itor != map.end(); ++itor)
                {
                    message = rmm.makeRegisterChannelMsg(itor->first, itor->second);
                    std::string reqChResp = udp->send(message);
                    std::cout << __PRETTY_FUNCTION__ << "[" << __LINE__ << "] res register channel: " << reqChResp << std::endl;
                }

                udp->quit();

                delete td;
                delete udp;
                return true;
            }

            delete udp;
            delete td;
        }
    }
    return false;
}

bool RequestChannelTestSuite::_test()
{
    ReqMsgMaker rmm;
    std::string message;
    std::string ip("127.0.0.1");
    int port = 6000; //default port
    int pid = RequestChannelTestSuite::_pid;

    rmm.setPid(pid);
    rmm.setProcessName(RequestChannelTestSuite::_psName);
    rmm.setChannelList(RequestChannelTestSuite::_keyList);

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
        }

        std::list<std::string>::iterator tarItor;
        tarItor = RequestChannelTestSuite::_targetKeyList.begin();

        while(tarItor != RequestChannelTestSuite::_targetKeyList.end())
        {
            message = rmm.makeRequestChannelMsg((*tarItor));
            reqChResp = udp->send(message);

            struct json_object* respJobj = json_tokener_parse(reqChResp.c_str());

            if(RequestChannelTestSuite::_verifyResponseOk(respJobj, (*tarItor), "ready") == false)
            {
                udp->quit();
                return false;
            }

            json_object_put(respJobj);

            ++tarItor;
        }

        udp->quit();
    }

    delete udp;
    return true;
}

bool RequestChannelTestSuite::_verifyResponseOk(struct json_object* jobj, std::string key, std::string state)
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
    }

    json_object_put(jobj);
    return true;
}
