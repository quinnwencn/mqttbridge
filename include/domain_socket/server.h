#pragma once

#include <string_view>
#include <functional>
#include <atomic>
#include <string>

namespace DomainSocket {

class DomainSocketServer {
public:
    using MessageCallback = std::function<int(std::string_view)>;

    DomainSocketServer(std::string_view socketPath);
    DomainSocketServer(std::string_view socketPath, MessageCallback messageCallback);
    ~DomainSocketServer();

    void Start();
    void Stop();

private:
    void Run();

    std::string socketPath_;
    int serverSocket_;
    std::atomic<bool> running_{false};
    MessageCallback messageCallback_;
};

}
