#include "domain_socket/server.h"

#include <string>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

namespace DomainSocket {

DomainSocketServer::DomainSocketServer(std::string_view socketPath) :
    socketPath_(socketPath), serverSocket_(-1), messageCallback_(nullptr) {}

DomainSocketServer::DomainSocketServer(std::string_view socketPath, MessageCallback messageCallback) :
    socketPath_(socketPath), serverSocket_(-1), messageCallback_(messageCallback) {}

DomainSocketServer::~DomainSocketServer() {
    Stop();
}

void DomainSocketServer::Start() {
    Run();
}

void DomainSocketServer::Stop() {
    running_.store(false);
    if (serverSocket_ != -1) {
        close(serverSocket_);
        serverSocket_ = -1;
        unlink(socketPath_.c_str());
    }
}

void DomainSocketServer::Run() {
    serverSocket_ = socket(AF_UNIX, SOCK_STREAM, 0);
    if (serverSocket_ < 0) {
        perror("socket");
        return;
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
        return;
    }

    if (listen(serverSocket_, 5) < 0) {
        perror("listen");
        close(serverSocket_);
        serverSocket_ = -1;
        return;
    }

    running_.store(true);
    while (running_.load()) {
        int clientFd = accept(serverSocket_, nullptr, nullptr);
        if (clientFd < 0) {
            if (running_.load()) {
                perror("accept");
            }
            continue;
        }

        std::array<char, 1024> buffer;
        ssize_t bytesRead = read(clientFd, buffer.data(), buffer.size());
        if (bytesRead > 0) {
            std::string message(buffer.data(), bytesRead);
            if (messageCallback_) {
                messageCallback_(message);
            }
        }

        close(clientFd);
    }
}

}
