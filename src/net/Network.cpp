#include "net/Network.hpp"

#include <gccore.h>
#include <network.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <cstring>

Network& Network::instance() {
    static Network network;
    return network;
}

bool Network::init() {
    char ip[16], gateway[16], netmask[16];

    s32 res = if_config(ip, netmask, gateway, true, 20);
    if (res < 0) return false;

    strncpy(mIp, ip, sizeof(mIp) - 1);
    mIp[sizeof(mIp) - 1] = '\0';
    mReady = true;
    return true;
}

bool Network::isReady() const { return mReady; }
const char* Network::getIP() const { return (const char*)mIp; }

s32 Network::connectTo(const char* ip) {
    if (!mReady) return -1;

    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(5000);
    server.sin_addr.s_addr = inet_addr(ip);

    s32 sock = net_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) return -1;

    if (net_connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        net_close(sock);
        return -1;
    }

    return sock;
}

void Network::disconnect(s32 sock) { net_close(sock); }

s32 Network::send(s32 sock, const void* data, u32 size) {
    return net_send(sock, data, size, 0);
}

s32 Network::recieve(s32 sock, void* buffer, u32 size) {
    return net_recv(sock, buffer, size, 0);
}

