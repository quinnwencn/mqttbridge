#include "domainsocket/server.h"

int main(int argc, char** argv) {
    DomainSocket::Server server("/tmp/mqttbridge_publish.sock", [](std::string_view message) {
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
