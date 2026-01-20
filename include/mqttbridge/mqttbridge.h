#pragma once

#include <mosquittopp.h>
#include <string_view>
#include <cstdint>
#include <memory>
#include <functional>
#include <thread>

#include "mqttbridge/imqttclient.h"
#include "mqttbridge/itransport.h"
#include "mqttbridge/config.h"


namespace MqttBridge {

class MqttBridge {
public:
    MqttBridge(const Config& config);
    ~MqttBridge();

    bool Start();
    void StartLoop();
    void Stop();

private:
    static std::function<void(int)> signalHandler_;
    static void SignalHandler(int sig);

    Config config_;
    std::unique_ptr<IMqttClient> mqttClient_;
    std::unique_ptr<ITransport> transport_;
    bool running_ = false;
    std::thread loopThread_;
};

} // namespace MqttBridge
