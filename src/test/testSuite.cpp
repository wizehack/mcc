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
    std::map<int, FuncPtr>::iterator itor;

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
