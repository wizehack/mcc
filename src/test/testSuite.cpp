#include "testSuite.h"
#include <iostream>

TestSuite::TestSuite():
    m_next(NULL),
    m_testFunc_cb(NULL),
    m_cbFuncMap(){}
TestSuite::~TestSuite(){}

void TestSuite::setNext(TestSuite* testSuite)
{
    this->m_next = testSuite;
}

bool TestSuite::exec(int tcId)
{
    if(tcId <= 0)
        return false;

    if(tcId > 0)
        this->m_testFunc_cb = m_cbFuncMap[tcId];

    if(this->m_testFunc_cb)
        return this->m_testFunc_cb();

    std::cout << "NOT FOUND TEST CASE" << std::endl;
    return false;
}

void TestSuite::add(int tcId, FuncPtr func_cb)
{
    if(func_cb)
        this->m_cbFuncMap.insert(std::pair<int, FuncPtr>(tcId, func_cb));
    else
        std::cout << "INVALID TEST CASE" << std::endl;
}

bool TestSuite::_verifyResponseError(struct json_object* jobj, mcHubd::RESPCODE respCode, std::string message)
{
    struct json_object* codeJobj = NULL;
    struct json_object* msgJobj = NULL;
    struct json_object* retJobj = NULL;
    mcHubd::RESPCODE code;

    if((jobj == NULL) || is_error(jobj))
        return false;

    if(!json_object_object_get_ex(jobj, "errorCode", &codeJobj))
        return false;

    code = static_cast<mcHubd::RESPCODE>(json_object_get_int(codeJobj));
    if(code != respCode)
        return false;

    if(!json_object_object_get_ex(jobj, "errorMessage", &msgJobj))
        return false;

    std::string errMsg(json_object_get_string(msgJobj));
    if(errMsg.compare(message) != 0)
        return false;

    if(!json_object_object_get_ex(jobj, "return", &retJobj))
        return false;

    if(json_object_get_boolean(retJobj))
        return false;

    return true;
}
