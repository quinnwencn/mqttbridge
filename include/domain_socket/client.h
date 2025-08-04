#include <string>
#include <vector>
#include <cstdint>

namespace DomainSocket {

class DomainSocketClient {
public:
    DomainSocketClient(std::string_view socketPath);
    ~DomainSocketClient();

    bool Connect();
    bool IsConnected() const;
    bool SendMsg(const std::vector<uint8_t>& mswomeng);
    bool ReceiveMsg(std::vector<uint8_t>& msg, size_t maxSize = 1024);

private:
    std::string socketPath_;
    int socketFd_;
    bool connected_{false};
};

}
