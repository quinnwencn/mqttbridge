#include "server.h"

#include <string>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

namespace DomainSocket {

Server::Server(std::string_view socketPath) :
    socketPath_(socketPath), serverSocket_(-1), messageCallback_(nullptr) {}

Server::Server(std::string_view socketPath, MessageCallback messageCallback) :
    socketPath_(socketPath), serverSocket_(-1), messageCallback_(messageCallback) {}

Server::~Server() {
    Stop();
}

bool Server::Start() {
    serverSocket_ = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (serverSocket_ < 0) {
        perror("socket");
        return false;
    }

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socketPath_.c_str(), sizeof(addr.sun_path) - 1);
    unlink(socketPath_.c_str()); 

    if (bind(serverSocket_, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
        perror("bind");
        close(serverSocket_);
        serverSocket_ = -1;
        return false;
    }

    running_.store(true);
    recvThread_ = std::thread(&Server::RecvLoop, this);
    return true;
}

void Server::Stop() {
    running_.store(false);

    if (serverSocket_ != -1) {
        shutdown(serverSocket_, SHUT_RDWR);
        close(serverSocket_);
        serverSocket_ = -1;
    }

    if (recvThread_.joinable()) {
        recvThread_.join();
    }

    unlink(socketPath_.c_str());
}

void Server::RecvLoop() {
    while (running_.load()) {
        char buffer[1024];
        ssize_t bytesRead = recv(serverSocket_, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0) {
            break;
        }

        if (messageCallback_) {
            std::string_view message(buffer, static_cast<size_t>(bytesRead));
            messageCallback_(message);
        }
    }

}

}
