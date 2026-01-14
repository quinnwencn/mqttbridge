#pragma once

#include <string_view>
#include <functional>
#include <atomic>
#include <thread>
#include <string>

namespace DomainSocket {

class Server {
public:
    using MessageCallback = std::function<void(std::string_view)>;

    Server(std::string_view socketPath);
    Server(std::string_view socketPath, uint16_t bufferSize);
    Server(std::string_view socketPath, MessageCallback messageCallback);
    Server(std::string_view socketPath, uint16_t bufferSize, MessageCallback messageCallback);
    ~Server();

    bool Start();
    void Stop();
    void SetMessageCallback(MessageCallback callback) {
        messageCallback_ = callback;
    }

private:
    void RecvLoop();

    int serverSocket_;
    uint16_t bufferSize_{2048};
    std::string socketPath_;
    std::atomic<bool> running_{false};
    std::thread recvThread_;
    MessageCallback messageCallback_;
};

}
