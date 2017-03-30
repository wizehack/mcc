#include "contract.h"

mcHubd::Contract::Contract(int contractID):
    m_processName(),
    m_clientKey(),
    m_pid(-1),
    m_channel(-1),
    m_respCode(MCHUBD_OK),
    m_contractID(contractID),
    m_reason(NOTI_CHANNEL_CLOSE),
    m_status(CLOSE)
{}


mcHubd::Contract::Contract():
    m_processName(),
    m_clientKey(),
    m_pid(-1),
    m_channel(-1),
    m_respCode(MCHUBD_OK),
    m_contractID(0),
    m_reason(NOTI_CHANNEL_CLOSE),
    m_status(CLOSE)
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

void mcHubd::Contract::setRespCode(mcHubd::RESPCODE code)
{
    this->m_respCode = code;
}

void mcHubd::Contract::setChannelStatus(mcHubd::STATUS status)
{
    this->m_status = status;
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

mcHubd::RESPCODE mcHubd::Contract::getRespCode() const
{
    return this->m_respCode;
}

mcHubd::STATUS mcHubd::Contract::getChannelStatus() const
{
    return this->m_status;
}
