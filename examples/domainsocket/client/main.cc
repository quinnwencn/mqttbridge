#include "domainsocket/client.h"

int main(int argc, char** argv) {
    DomainSocket::Client client("/tmp/domain_socket_example");

    if (!client.Connect()) {
        printf("Failed to connect to the server.\n");
        return 1;
    }

    printf("Connected to the server.\n");
    std::string message = "Hello from client!";
    if (!client.SendMsg(message)) {
        printf("Failed to send message.\n");
        return 1;
    }

    printf("Message sent to the server.\n");

    return 0;
}
