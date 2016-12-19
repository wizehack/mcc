#include "contract.h"

mcHubd::Contract::Contract(int contractID):
    m_processName(),
    m_clientKey(),
    m_pid(-1),
    m_channel(-1),
    m_respCode(MCHUBD_OK),
    m_contractID(contractID),
    m_reason(NOTI_INIT)
{}


mcHubd::Contract::Contract():
    m_processName(),
    m_clientKey(),
    m_pid(-1),
    m_channel(-1),
    m_respCode(MCHUBD_OK),
    m_contractID(0),
    m_reason(NOTI_INIT)
{}
mcHubd::Contract::~Contract(){}

void mcHubd::Contract::setProcessName(std::string name)
{
    this->m_processName = name;
}

void mcHubd::Contract::setClientKey(std::string name)
{
    this->m_clientKey = name;
}

void mcHubd::Contract::setProcessId(pid_t pid)
{
    this->m_pid = pid;
}

void mcHubd::Contract::setChannel(key_t channel)
{
    this->m_channel = channel;
}

void mcHubd::Contract::setContractReason(mcHubd::CONTRACTREASON reason)
{
    this->m_reason = reason;
}

void mcHubd::Contract::setRespCode(mcHubd::RESPCODE code)
{
    this->m_respCode = code;
}

std::string mcHubd::Contract::getProcessName() const
{
    return this->m_processName;
}

std::string mcHubd::Contract::getClientKey() const
{
    return this->m_clientKey;
}

pid_t mcHubd::Contract::getProcessId() const
{
    return this->m_pid;
}

key_t mcHubd::Contract::getChannel() const
{
    return this->m_channel;
}

int mcHubd::Contract::getContractID() const
{
    return this->m_contractID;
}

mcHubd::CONTRACTREASON mcHubd::Contract::getContractReason() const
{
    return this->m_reason;
}

mcHubd::RESPCODE mcHubd::Contract::getRespCode() const
{
    return this->m_respCode;
}
