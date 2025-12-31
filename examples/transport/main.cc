#include <iostream>
#include <string_view>

#include "transport/domainsocket_transport.h"

using namespace MqttBridge;
using namespace std;

int main(int argc, char** argv) {
    constexpr std::string_view someToml = R"(
        [transport]
        type = "domainsocket"
        node_name = "example_node"
        publish_topic = "/tmp/domain_socket_transport"
        subscribe_topic = "/tmp/domain_socket_transport"
    )"sv;
    TransportConfig config;
    auto tbl = toml::parse(someToml);
    config.LoadFromTable(*tbl["transport"].as_table());

    DomainSocketTransport transport(config);

    transport.SetMessageCallback([](string_view msg) {
        cout << "Received message: " << msg << endl;
    });

    transport.Start();

    Message message;
    message.topic = config.PublishTopic();
    message.json = R"({"message": "Hello from DomainSocketTransport!"})";

    if (transport.Send(message)) {
        cout << "Message sent successfully." << endl;
    } else {
        cout << "Failed to send message." << endl;
    }

    transport.Stop();

    return 0;
}