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

    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(this->m_ipAddr.c_str());
    servAddr.sin_port = htons(this->m_port);

    this->m_socket = sock;
    this->m_servaddr = servAddr;
    return true;
}

std::string UDPClient::send(std::string data)
{
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

    wrap_recvfrom(this->m_socket, (void *)&rsp, sizeof(rsp), 0,
                            (struct sockaddr *)&(m_servaddr), &serverLen);
    ack.assign(rsp);
    std::cout << __FUNCTION__ << "[" << __LINE__ << "] ack: " << rsp << std::endl;

    memset(rsp, '\0', MAX_SIZE);

    wrap_recvfrom(this->m_socket, (void *)&rsp, sizeof(rsp), 0,
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

std::string UDPClient::receive()
{
    char rcv[MAX_SIZE];
    std::string ret;

    char rsp[MAX_SIZE]; //ack
    std::string ack;

    socklen_t serverLen = sizeof(m_servaddr);

    memset(rcv, '\0', MAX_SIZE);
    wrap_recvfrom(this->m_socket, (void *)&rcv, sizeof(rcv), 0,
                            (struct sockaddr *)&(m_servaddr), &serverLen);
    ret.assign(rcv);

    ack = UDPClient::makeAck(ret);

    memset(rsp, '\0', MAX_SIZE);
    (char*)memcpy(rsp, ack.c_str(), ack.length());
    rsp[ack.length()] = '\0';

    sendto(this->m_socket, (void *)rsp, strlen(rsp), 0, (struct sockaddr *)&m_servaddr, sizeof(m_servaddr));

    return ret;
}

std::string UDPClient::getSubscribe()
{
    char rcv[MAX_SIZE];
    std::string ret;

    char rsp[MAX_SIZE]; //ack
    std::string ack;

    socklen_t serverLen = sizeof(m_servaddr);

    memset(rcv, '\0', MAX_SIZE);
    recvfrom(this->m_socket, (void *)&rcv, sizeof(rcv), 0,
                            (struct sockaddr *)&(m_servaddr), &serverLen);
    ret.assign(rcv);

    ack = UDPClient::makeAck(ret);

    memset(rsp, '\0', MAX_SIZE);
    (char*)memcpy(rsp, ack.c_str(), ack.length());
    rsp[ack.length()] = '\0';

    sendto(this->m_socket, (void *)rsp, strlen(rsp), 0, (struct sockaddr *)&m_servaddr, sizeof(m_servaddr));

    return ret;
}

std::string UDPClient::makeAck(std::string& msg)
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

bool UDPClient::isSubscription(std::string& msg)
{
    struct json_object* jobj = NULL;
    struct json_object* subJobj = NULL;

    jobj = json_tokener_parse(msg.c_str());

    if((jobj == NULL) || is_error(jobj))
        return false;

    if(!json_object_object_get_ex(jobj, "subscribe", &subJobj))
    {
        json_object_put(jobj);
        return false;
    }

    return json_object_get_boolean(subJobj);
}

ssize_t UDPClient::wrap_recvfrom(int sockfd, void *buf, size_t len, int flags,
                                 struct sockaddr *src_addr, socklen_t *addrlen)
{
    size_t size = recvfrom(sockfd, buf, len, flags, src_addr, addrlen);
    std::string msg;

    if(buf)
    {
        msg.assign((char*)buf);
    }

    while(UDPClient::isSubscription(msg))
    {
        size = recvfrom(sockfd, buf, len, flags, src_addr, addrlen);
    }

    return size;
}
