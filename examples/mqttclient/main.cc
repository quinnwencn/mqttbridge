#include <iostream>
#include <thread>

#include "mqttbridge/imqttclient.h"
#include "mqttclient/mosquitto_mqttclient.h"
#include "mqttbridge/logger.h"

using namespace MqttBridge;
using namespace std;

int main(int argc, char** argv) {
    constexpr std::string_view someToml = R"(
        [mqtt]
        host = "localhost"
        port = 1883
        client_id = "example_mqtt_client"
        subscribe_topic = "example/subscribe"
        publish_topic = "example/subscribe"
        enable_tls = false
        verify_peer = false
        qos = 1
        retain = false  
    )"sv;

    auto tbl = toml::parse(someToml);
    MqttConfig config;
    config.LoadFromTable(*tbl.at("mqtt").as_table());
    config.WriteToLog("MqttConfig");

    MosquittoMqttClient mqttClient(config);

    mqttClient.SetMessageCallback(
        [](const Message& msg) {
            cout << "Received message on topic: " << msg.topic << " with payload: " << msg.json << endl;
        }
    );

    if (!mqttClient.Start()) {
        cerr << "Failed to start MQTT client" << endl;
        return 1;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Wait for connection to establish
    mqttClient.Publish(config.PublishTopic(), Message{config.PublishTopic(), R"({"message": "Hello, MQTT!"})"}, config.Qos(), config.Retain());

    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Wait to receive messages
    LOG_TRACE("Mqtt client exiting...");

    mqttClient.Stop();
    return 0;
}
