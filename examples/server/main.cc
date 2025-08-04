#include "domain_socket/server.h"

int main(int argc, char** argv) {
    DomainSocket::DomainSocketServer server("/tmp/domain_socket_example", [](std::string_view message) -> int {
        printf("Received message: %.*s\n", static_cast<int>(message.size()), message.data());
        
        return 0;
    });

    server.Start();

    return 0;
}