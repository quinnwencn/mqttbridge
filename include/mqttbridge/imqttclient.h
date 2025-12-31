#pragma once

#include <string_view>
#include <functional>

#include "message.h"

namespace MqttBridge {

class IMqttClient {
public:
    using MessageCallback = std::function<void(const Message&)>;
    
    virtual ~IMqttClient() = default;

    virtual bool Publish(std::string_view topic, const Message& message, int qos, bool retain) = 0;
    virtual bool Subscribe(std::string_view topic, int qos) = 0;
    virtual void SetMessageCallback(MessageCallback callback) = 0;

    virtual bool Start() = 0;
    virtual void Stop() = 0;
    virtual bool ConfigureTls() = 0;
    virtual bool IsConnected() const = 0;
    virtual void Disconnect() = 0;
};
    
}