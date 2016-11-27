#include "tester.h"
#include <iostream>

Tester::Tester():
    m_next(NULL){}
Tester::~Tester(){}

void Tester::setNext(Tester* tester)
{
    this->m_next = tester;
}

bool Tester::exec(int tcId)
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

void Tester::add(int tcId, FuncPtr func_cb)
{
    if(func_cb)
        this->m_cbFuncMap.insert(std::pair<int, FuncPtr>(tcId, func_cb));
    else
        std::cout << "INVALID TEST CASE" << std::endl;
}
