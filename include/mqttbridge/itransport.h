#pragma once

#include <cstdint>
#include <string_view>
#include <functional>
#include <memory>

#include "message.h"
#include "config.h"

namespace MqttBridge {

    class ITransport {
    public:
        using MessageCallback = std::function<void(const std::string_view&)>;
        
        virtual ~ITransport() = default;

        virtual bool Start() = 0;
        virtual void Stop() = 0;
        virtual void SetMessageCallback(MessageCallback callback) = 0;
        virtual bool Send(const Message& message) = 0;
    };

    std::unique_ptr<ITransport> CreateTransport(std::string_view type, const TransportConfig& config);
}
