#include "domainsocket/client.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: %s <socket_path>\n", argv[0]);
        return 1;
    }

    DomainSocket::Client client(argv[1]);

    if (!client.Connect()) {
        printf("Failed to connect to the server.\n");
        return 1;
    }

    printf("Connected to the server.\n");
    std::string message = "Hello from client, test hook!";
    if (!client.SendMsg(message)) {
        printf("Failed to send message.\n");
        return 1;
    }

    printf("Message sent to the server.\n");

    return 0;
}
