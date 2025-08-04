#include "domain_socket/client.h"

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

namespace DomainSocket {

DomainSocketClient::DomainSocketClient(std::string_view socketPath) :
    socketPath_(socketPath), socketFd_(-1), connected_(false) {}

DomainSocketClient::~DomainSocketClient() {
    if (connected_) {
        close(socketFd_);
    }
}

bool DomainSocketClient::Connect() {
    socketFd_ = socket(AF_UNIX, SOCK_STREAM, 0);
    if (socketFd_ < 0) {
        return false;
    }

    sockaddr_un addr{};
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socketPath_.data(), sizeof(addr.sun_path) - 1);

    if (connect(socketFd_, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
        close(socketFd_);
        socketFd_ = -1;
        return false;
    }

    connected_ = true;
    return true;
}

bool DomainSocketClient::IsConnected() const {
    return connected_;
}

bool DomainSocketClient::SendMsg(const std::vector<uint8_t>& msg) {
    if (!connected_) {
        return false;
    }

    ssize_t bytesSent = send(socketFd_, msg.data(), msg.size(), 0);
    return bytesSent == static_cast<ssize_t>(msg.size());
}

bool DomainSocketClient::ReceiveMsg(std::vector<uint8_t>& msg, size_t maxSize) {
    if (!connected_) {
        return false;
    }

    msg.resize(maxSize);
    ssize_t bytesReceived = recv(socketFd_, msg.data(), maxSize, 0);
    if (bytesReceived < 0) {
        return false;
    }

    msg.resize(bytesReceived);
    return true;
}

}
