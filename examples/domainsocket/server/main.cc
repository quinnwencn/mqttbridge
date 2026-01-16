#include "domainsocket/server.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: %s <socket_path>\n", argv[0]);
        return 1;
    }

    DomainSocket::Server server(argv[1], [](std::string_view message) {
        printf("Received message: %.*s\n", static_cast<int>(message.size()), message.data());
    });

    server.Start();

    while (true) {
        // Keep the server running
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    server.Stop();

    return 0;
}
