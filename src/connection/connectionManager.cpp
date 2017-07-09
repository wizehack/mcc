#include "connectionManager.h"
#include "feature/connectionInfo.h"
#include "feature/message.h"
#include "feature/messageProcessor.h"
#include "udpServer.h"

std::atomic<mcHubd::ConnectionManager*> mcHubd::ConnectionManager::_singleton;
std::mutex mcHubd::ConnectionManager::_mutex;

mcHubd::ConnectionManager::ConnectionManager():
    m_udpServer(NULL),
    m_port(6000)
{
    m_udpServer = new mcHubd::UDPServer(this->m_port);
    if(m_udpServer)
        mcHubd::ConnectionInfo::getInstance()->registerServer(m_udpServer);
}

mcHubd::ConnectionManager::~ConnectionManager()
{
    if(m_udpServer)
        delete m_udpServer;
}

mcHubd::ConnectionManager* mcHubd::ConnectionManager::getInstance() {
    mcHubd::ConnectionManager* inst = _singleton.load(std::memory_order_relaxed);
    std::atomic_thread_fence(std::memory_order_acquire);
    if(inst == 0) {
        std::lock_guard<std::mutex> lock(_mutex);
        inst = _singleton.load(std::memory_order_relaxed);
        if(inst == 0) {
            inst = new mcHubd::ConnectionManager();
            std::atomic_thread_fence(std::memory_order_release);
            _singleton.store(inst, std::memory_order_relaxed);
        }
    }
    return inst;
}

void mcHubd::ConnectionManager::start()
{
    std::cout << __PRETTY_FUNCTION__ << "[" << __LINE__ << "]: listen" << std::endl;
    std::thread acceptHandler(mcHubd::ConnectionManager::_listen);
    acceptHandler.detach();
}

mcHubd::UDPServer* mcHubd::ConnectionManager::getUDPServer()
{
    return this->m_udpServer;
}

mcHubd::Message* mcHubd::ConnectionManager::_getMessageObject(std::shared_ptr<mcHubd::SocketData> sockData)
{
    struct json_object* jobj = NULL;
    struct json_object* typeJobj = NULL;
    struct json_object* msgJobj = NULL;
    mcHubd::MSGTYPE msgType = mcHubd::UNKNOWN_MSG_TYPE;
    mcHubd::Message* msg = NULL;
    struct sockaddr_in clientAddr = sockData->getSockaddr();
    std::string payload = sockData->getPayload();;
    std::string type;
    std::string body;

    jobj = json_tokener_parse(payload.c_str());

    if((jobj == NULL) || is_error(jobj))
        return msg;

    if(!json_object_object_get_ex(jobj, "msgType", &typeJobj))
    {
        json_object_put(jobj);
        return msg;
    }

    if(!json_object_object_get_ex(jobj, "message", &msgJobj))
    {
        json_object_put(jobj);
        return msg;
    }

    if(typeJobj)
        type.assign(json_object_get_string(typeJobj));
    if(msgJobj)
        body.assign(json_object_get_string(msgJobj));

    if(type.compare("RegisterClient") == 0)
        msgType = mcHubd::REQ_REG_CLIENT;
    else if(type.compare("RegisterChannel") == 0)
        msgType = mcHubd::REQ_REG_CHANNEL;
    else if(type.compare("DeleteClient") == 0)
        msgType = mcHubd::REQ_DEL_CLIENT;
    else if(type.compare("DeleteChannel") == 0)
        msgType = mcHubd::REQ_DEL_CHANNEL;
    else if(type.compare("GetChannel") == 0)
        msgType = mcHubd::REQ_GET_CHANNEL;

    msg = new mcHubd::Message(msgType);

    if(msg)
    {
        msg->setBody(body);
        msg->setConn(clientAddr);
    }

    json_object_put(jobj);

    return msg;
}

bool mcHubd::ConnectionManager::_processMessage(mcHubd::Message* msg)
{
    return mcHubd::MessageProcessor::request(msg);
}

void mcHubd::ConnectionManager::_listen()
{
    UDPServer* udpServer = mcHubd::ConnectionManager::getInstance()->getUDPServer();

    if(udpServer)
    {
        udpServer->start();

        while(1)
        {
            fflush(stdout);
            std::cout << __PRETTY_FUNCTION__ << " [" << __LINE__ << "] " << std::endl;
            std::shared_ptr<mcHubd::SocketData> sockData = udpServer->recv();

            if(sockData)
            {
                mcHubd::Message* msg = mcHubd::ConnectionManager::_getMessageObject(sockData);

                if(msg)
                {
                    if(mcHubd::ConnectionManager::_processMessage(msg) == false)
                    {
                        std::cout << __PRETTY_FUNCTION__ << "[" << __LINE__ << "] " << "ERROR: msg is not processed" << std::endl;
                    }

                    delete msg;
                }
            }
        }
    }
}
