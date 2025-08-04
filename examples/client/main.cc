#include "domain_socket/client.h"

int main(int argc, char** argv) {
    DomainSocket::DomainSocketClient client("/tmp/domain_socket_example");

    if (!client.Connect()) {
        printf("Failed to connect to the server.\n");
        return 1;
    }

    printf("Connected to the server.\n");
    std::vector<uint8_t> message = {'H', 'e', 'l', 'l', 'o', ' ', 'S', 'e', 'r', 'v', 'e', 'r'};
    if (!client.SendMsg(message)) {
        printf("Failed to send message.\n");
        return 1;
    }

    printf("Message sent to the server.\n");

    return 0;
}
