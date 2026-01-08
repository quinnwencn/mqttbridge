#include <signal.h>
#include <unistd.h>

#include "mqttbridge/mqttbridge.h"
#include "mqttclient/mosquitto_mqttclient.h"
#include "transport/domainsocket_transport.h"
#include "mqttbridge/logger.h"

namespace MqttBridge {

std::function<void(int)> MqttBridge::signalHandler_ = nullptr;

MqttBridge::MqttBridge(const Config& config) :
    config_(config),
    mqttClient_(std::make_unique<MosquittoMqttClient>(config.Mqtt())),
    transport_(CreateTransport(config.Transport().Type(), config.Transport())),
    mqttToTransportQueue_(),
    transportToMqttQueue_(),
    running_(false) {

    mqttClient_->SetMessageCallback(
        [this](const Message& msg) {
            LOG_TRACE("Received message from MQTT on topic {}:", msg.topic, msg.json);
            mqttToTransportQueue_.Push(msg);
        }
    );

    transport_->SetMessageCallback(
        [this](const std::string_view& msgStr) {
            LOG_TRACE("Received message from Transport: {}", msgStr);
            Message msg;
            msg.json = std::string(msgStr);
            transportToMqttQueue_.Push(msg);
        }
    );

    signalHandler_ = [this](int sig) {
        LOG_INFO("Received signal {}, shutting down...", sig);
        Stop();
    };
}

MqttBridge::~MqttBridge() {
    Stop();
    if (loopThread_.joinable()) {
        loopThread_.join();
    }
}

bool MqttBridge::Start() {
    if (!mqttClient_->Start()) {
        LOG_FATAL("Failed to start MQTT client");
        return false;
    }

    if (!transport_->Start()) {
        LOG_FATAL("Failed to start transport");
        return false;
    }

    signal(SIGINT, &MqttBridge::SignalHandler);
    signal(SIGTERM, &MqttBridge::SignalHandler);

    running_ = true;
    while (running_) {
        Message msgFromMqtt;
        bool mqttToTransportQueueNotEmpty = mqttToTransportQueue_.TryPop(msgFromMqtt);
        if (mqttToTransportQueueNotEmpty) {
            if (!transport_->Send(msgFromMqtt)) {
                LOG_ERROR("Failed to send message from MQTT to transport");
            }
        }

        Message msgFromTransport;
        bool transportToMqttQueueNotEmpty = transportToMqttQueue_.TryPop(msgFromTransport);
        if (transportToMqttQueueNotEmpty) {
            if (!mqttClient_->Publish(config_.Mqtt().PublishTopic(), msgFromTransport, config_.Mqtt().Qos(), config_.Mqtt().Retain())) {
                LOG_ERROR("Failed to publish message from transport to MQTT");
            }
        }

        if (!mqttToTransportQueueNotEmpty || !transportToMqttQueueNotEmpty) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    return true;
}

void MqttBridge::StartLoop() {
    loopThread_ = std::thread([this]() {
        LOG_TRACE("Starting MQTT Bridge loop...");
        Start();
    });
}

void MqttBridge::Stop() {
    if (running_) {
        running_ = false;
        LOG_INFO("Stopping MQTT Bridge...");
        transport_->Stop();
        mqttClient_->Stop();
    }
}

void MqttBridge::SignalHandler(int sig) {
    if (signalHandler_) {
        signalHandler_(sig);
    }
}

}
