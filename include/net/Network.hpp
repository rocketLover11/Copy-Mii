#pragma once

#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <gccore.h>
#include <network.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class Network {
public:
    static Network& instance();

    bool init();
    bool isReady() const;
    const char* getIP() const;

    s32 connectTo(const char* ip);
    void disconnect(s32 sock);

    s32 send(s32 sock, const void* data, u32 size);
    s32 recieve(s32 sock, void* buffer, u32 size);

private:
    Network() = default;

    bool mReady = false;
    char mIp[16];
};

#endif