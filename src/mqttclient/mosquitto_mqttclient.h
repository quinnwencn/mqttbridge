#pragma once

#include <mosquittopp.h>
#include <cstdint>

#include "mqttbridge/imqttclient.h"
#include "mqttbridge/config.h"

namespace MqttBridge {

class MosquittoMqttClient : public IMqttClient, public mosqpp::mosquittopp {
public:
    MosquittoMqttClient(const MqttConfig& config);
    MosquittoMqttClient(const MqttConfig& config, MessageCallback callback);
    ~MosquittoMqttClient() override;

    bool Publish(std::string_view topic, const Message& message, int qos = 1, bool retain = false) override;
    bool Subscribe(std::string_view topic, int qos) override;
    void SetMessageCallback(MessageCallback callback) override;

    bool Start() override;
    void Stop() override;
    bool ConfigureTls() override;
    bool IsConnected() const override;
    void Disconnect() override;

protected:
    void on_connect(int rc) override;
    void on_message(const struct mosquitto_message* msg) override;
    void on_disconnect(int rc) override;

    bool connected_{false};
    MqttConfig config_;
    MessageCallback messageCallback_;
};

}  // namespace MqttBridge
