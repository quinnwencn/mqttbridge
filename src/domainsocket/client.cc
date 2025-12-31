#include "client.h"

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

namespace DomainSocket {

Client::Client(std::string_view socketPath) :
    socketPath_(socketPath), socketFd_(-1) {}

Client::~Client() {
    if (socketFd_ != -1) {
        close(socketFd_);
    }
}

bool Client::Connect() {
    if (socketFd_ != -1) {
        return true; // Already connected
    }

    socketFd_ = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (socketFd_ < 0) {
        perror("socket error");
        return false;
    }

    memset(&addr_, 0, sizeof(addr_));
    addr_.sun_family = AF_UNIX;
    strncpy(addr_.sun_path, socketPath_.data(), sizeof(addr_.sun_path) - 1);

    return true;
}

bool Client::SendMsg(const std::vector<uint8_t>& msg) {
    ssize_t sentNum = sendto(socketFd_, 
                             msg.data(), 
                             msg.size(), 
                             0,
                             reinterpret_cast<struct sockaddr*>(&addr_),
                             sizeof(addr_));
    return sentNum == static_cast<ssize_t>(msg.size());
}

bool Client::SendMsg(const std::string& msg) {
    ssize_t sentNum = sendto(socketFd_, 
                             msg.data(), 
                             msg.size(), 
                             0,
                             reinterpret_cast<struct sockaddr*>(&addr_),
                             sizeof(addr_));
    return sentNum == static_cast<ssize_t>(msg.size());
}

}