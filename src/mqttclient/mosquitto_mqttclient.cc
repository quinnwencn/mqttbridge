#include "mosquitto_mqttclient.h"
#include "mqttbridge/logger.h"

namespace MqttBridge {

MosquittoMqttClient::MosquittoMqttClient(const MqttConfig& config) : MosquittoMqttClient(config, nullptr) {

}

MosquittoMqttClient::MosquittoMqttClient(const MqttConfig& config, MessageCallback callback) : 
    mosqpp::mosquittopp(config.ClientId().c_str()), 
    connected_(false),
    config_(config), 
    messageCallback_(callback) {
    mosqpp::lib_init();

    // Set username and password if provided
    if (!config_.Username().empty()) {
        username_pw_set(config_.Username().c_str(), config_.Password().c_str());
    }
}

MosquittoMqttClient::~MosquittoMqttClient() {
    Stop();
    mosqpp::lib_cleanup();
}

bool MosquittoMqttClient::Publish(std::string_view topic, const Message& message, int qos, bool retain) {
    int rc = publish(nullptr, 
                    topic.data(), 
                    static_cast<int>(message.json.size()), 
                    message.json.data(), 
                    qos, 
                    retain);
    
    if (rc != MOSQ_ERR_SUCCESS) {
        LOG_ERROR("Failed to publish message to topic: {}, error code: {}", topic, rc);
        return false;
    }

    LOG_DEBUG("Published message to topic: {}", topic);
    return true;
}

bool MosquittoMqttClient::Subscribe(std::string_view topic, int qos) {
    int rc = subscribe(nullptr, topic.data(), qos);
    if (rc != MOSQ_ERR_SUCCESS) {
        // Handle subscription error
        LOG_ERROR("Failed to subscribe to topic: {}, error code: {}", topic, rc);
        return false;
    } 

    LOG_DEBUG("Subscribed to topic: {}", topic);
    return true;
}

void MosquittoMqttClient::SetMessageCallback(MessageCallback callback) {
    messageCallback_ = callback;
}

bool MosquittoMqttClient::Start() {
    if (config_.EnableTls()) {
        if (!ConfigureTls()) {
            return false;
        }
    }

    int rc = connect_async(config_.Host().c_str(), config_.Port(), 60);
    if (rc != MOSQ_ERR_SUCCESS) {
        return false;
    }

    rc = loop_start();
    if (rc != MOSQ_ERR_SUCCESS) {
        return false;
    }

    return true;
}

void MosquittoMqttClient::Stop() {
    if (IsConnected()) {
        disconnect();
    }

    loop_stop(true);
}

bool MosquittoMqttClient::ConfigureTls() {
    if (config_.EnableTls()) {
        int rc = tls_set(config_.CaCert().c_str(),
                         nullptr, 
                         config_.ClientCert().empty() ? nullptr : config_.ClientCert().c_str(),
                         config_.ClientKey().empty() ? nullptr : config_.ClientKey().c_str(),
                         nullptr);
        if (rc != MOSQ_ERR_SUCCESS) {
            return false;
        }
        if (config_.VerifyPeer()) {
            tls_insecure_set(false);
        }
    }
    return true;
}

bool MosquittoMqttClient::IsConnected() const {
    return connected_;
}

void MosquittoMqttClient::Disconnect() {
    mosqpp::mosquittopp::disconnect();
}

void MosquittoMqttClient::on_connect(int rc) {
    if (rc == 0) {
        // Successfully connected
        connected_ = true;
        LOG_DEBUG("Connected to MQTT broker");

        if (!Subscribe(config_.SubscribeTopic().data(), config_.Qos())) {
            LOG_ERROR("Failed to subscribe to topic: {}", config_.SubscribeTopic());
        }
    } else {
        // Handle connection error
        LOG_ERROR("Failed to connect to MQTT broker, return code: {}", rc);
    }
}

void MosquittoMqttClient::on_disconnect(int rc) {
    connected_ = false;
    LOG_DEBUG("Disconnected from MQTT broker with return code: {}", rc);
}

void MosquittoMqttClient::on_message(const struct mosquitto_message* msg) {
    if (messageCallback_) {
        Message message;
        message.topic = msg->topic;
        message.json = std::string(static_cast<const char*>(msg->payload), msg->payloadlen);
        messageCallback_(message);
    }
}

}
