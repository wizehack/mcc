#include <iostream>
#include "deleteClientTestSuite.h"
#include "testData.h"
#include "reqMsgMaker.h"
#include "udpClient.h"

std::string DeleteClientTestSuite::_testDataPath("");
std::string DeleteClientTestSuite::_psName("");
std::list<std::string> DeleteClientTestSuite::_keyList;

DeleteClientTestSuite::DeleteClientTestSuite() : TestSuite(){}
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
        this->add(1, DeleteClientTestSuite::_test);
    }
    else
    {
        std::cout << "Precondition Failed" << std::endl;
    }
}

bool DeleteClientTestSuite::_setPrecondition()
{
    if(DeleteClientTestSuite::_testDataPath.empty() == false)
    {
        TestData* td = new TestData(DeleteClientTestSuite::_testDataPath);
        DeleteClientTestSuite::_psName = td->getProcessName();
        DeleteClientTestSuite::_keyList = td->getKeyList();
        delete td;

        // request channel status

        return true;
    }
    return false;
}

bool DeleteClientTestSuite::_test()
{
    ReqMsgMaker rmm;
//    std::string regClientMsg;
    std::string delClientMsg;
    std::string ip("127.0.0.1");
    int port = 6000; //default port
    int pid = getpid();
    const int BUF_SIZE = 4096;

    rmm.setPid(pid);
    rmm.setProcessName(DeleteClientTestSuite::_psName);
    rmm.setChannelList(DeleteClientTestSuite::_keyList);

//    regClientMsg = rmm.makeRegisterClientMsg();
    delClientMsg = rmm.makeDeleteClientMsg();

    UDPClient* udp = new UDPClient(ip, port, BUF_SIZE);

    if(udp->conn())
    {
//        udp->send(regClientMsg);
        std::string response = udp->send(delClientMsg);
        struct json_object* jobj = json_tokener_parse(response.c_str());

        if(DeleteClientTestSuite::_verifyResponseOk(jobj) == false)
        {
            udp->quit();
            delete udp;
            return false;
        }

        udp->quit();
    }
    delete udp;
    return true;
}

bool DeleteClientTestSuite::_verifyResponseOk(struct json_object* jobj)
{
    struct json_object* codeJobj = NULL;
    struct json_object* retJobj = NULL;
    struct json_object* messageJobj = NULL;
    struct json_object* psNameJobj = NULL;
    struct json_object* pidJobj = NULL;
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

    if(!json_object_object_get_ex(messageJobj, "psName", &psNameJobj))
    {
        json_object_put(jobj);
        return false;
    }

    if(!json_object_object_get_ex(messageJobj, "pid", &pidJobj))
    {
        json_object_put(jobj);
        return false;
    }

    if(!psNameJobj || !pidJobj)
    {
        json_object_put(jobj);
        return false;
    }

    json_object_put(jobj);
    return true;
}
