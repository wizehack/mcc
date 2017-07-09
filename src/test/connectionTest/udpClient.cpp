#include "udpClient.h"
#include "mcHubType.h"

UDPClient::UDPClient(std::string ipAddr, int port, const int MAXBUF) :
    SocketClient(ipAddr, port, MAXBUF),
    m_servaddr()
{
//    memset(m_servaddr, 0, sizeof(m_servaddr));
}
UDPClient::~UDPClient(){}

bool UDPClient::conn()
{
    int sock;
    struct sockaddr_in servAddr;

    sock = socket(PF_INET, SOCK_DGRAM, 0);

    if(sock == -1)
    {
        std::cout << "Can not open socket" << std::endl;
        return false;
    }

//    memset(servAddr, 0, sizeof(m_servaddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(this->m_ipAddr.c_str());
    servAddr.sin_port = htons(this->m_port);

    this->m_socket = sock;
    this->m_servaddr = servAddr;
    return true;
}

std::string UDPClient::send(std::string data)
{
//    int len = 0;
    char req[MAX_SIZE];
    char rsp[MAX_SIZE];
    std::string ack;
    std::string response;

    socklen_t serverLen = sizeof(m_servaddr);

    memset(req, '\0', MAX_SIZE);
    (char*)memcpy(req, data.c_str(), data.length());
    req[data.length()] = '\0';

    std::cout << __FUNCTION__ << "[" << __LINE__ << "] request: " << req << std::endl;
    sendto(this->m_socket, (void *)req, strlen(req), 0, (struct sockaddr *)&m_servaddr, sizeof(m_servaddr));

    memset(rsp, '\0', MAX_SIZE);

    recvfrom(this->m_socket, (void *)&rsp, sizeof(rsp), 0,  
                            (struct sockaddr *)&(m_servaddr), &serverLen);
    ack.assign(rsp);
    std::cout << __FUNCTION__ << "[" << __LINE__ << "] ack: " << rsp << std::endl;

    memset(rsp, '\0', MAX_SIZE);

    recvfrom(this->m_socket, (void *)&rsp, sizeof(rsp), 0,  
                            (struct sockaddr *)&(m_servaddr), &serverLen);

    std::cout << __FUNCTION__ << "[" << __LINE__ << "] response: " << rsp << std::endl;

    response.assign(rsp);

    ack.clear();
    ack = UDPClient::makeAck(response);
    memset(req, '\0', MAX_SIZE);
    (char*)memcpy(req, ack.c_str(), ack.length());
    req[ack.length()] = '\0';

    sendto(this->m_socket, (void *)req, strlen(req), 0, (struct sockaddr *)&m_servaddr, sizeof(m_servaddr));

    return response;
}

std::string UDPClient::makeAck(std::string msg)
{
    struct json_object* jobj = NULL;
    struct json_object* typeJobj = NULL;
    std::string ackMsg;
 
    jobj = json_tokener_parse(msg.c_str());

    if((jobj == NULL) || is_error(jobj))
        return ackMsg;

    if(!json_object_object_get_ex(jobj, "msgType", &typeJobj))
    {
        json_object_put(jobj);
        return ackMsg;
    }

    json_object_put(typeJobj);

    typeJobj = json_object_new_string("ACK");
    json_object_object_add(jobj, "msgType", typeJobj);
    ackMsg.assign(json_object_get_string(jobj));

    return ackMsg;
}
