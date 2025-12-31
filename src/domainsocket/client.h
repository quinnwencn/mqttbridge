#include <string>
#include <vector>
#include <cstdint>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

namespace DomainSocket {

class Client {
public:
    Client(std::string_view socketPath);
    ~Client();

    bool Connect();
    bool SendMsg(const std::vector<uint8_t>& mswomeng);
    bool SendMsg(const std::string& msg);

private:
    std::string socketPath_;
    int socketFd_;
    struct sockaddr_un addr_;
};

}
