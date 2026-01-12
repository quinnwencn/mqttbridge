#pragma once

#include <string>
#include <cstdint>
#include <filesystem>
#include <toml++/toml.hpp>

namespace MqttBridge {

class IConfig {
public:
    virtual ~IConfig() = default;

    virtual void LoadFromTable(const toml::table& tbl) = 0;
    virtual void WriteToLog(std::string_view name) = 0;
};

class LogConfig : public IConfig {
public:
    LogConfig() = default;
    ~LogConfig() override = default;

    const std::filesystem::path& LogDir() const { return logDirs_; }
    int LogLevel() const { return logLevel_; }
    void LoadFromTable(const toml::table &tbl) override;
    void WriteToLog(std::string_view name) override;

private:
    std::filesystem::path logDirs_;
    int logLevel_;
};

class MqttConfig : public IConfig {
public:
    MqttConfig() = default;
    ~MqttConfig() override = default;

    const std::string& Host() const { return host_; }
    uint16_t Port() const { return port_; }
    int Qos() const { return qos_; }
    const std::string& ClientId() const { return clientId_; }
    const std::string& Username() const { return username_; }
    const std::string& Password() const { return password_; }
    bool EnableTls() const { return enableTls_; }
    bool VerifyPeer() const { return verifyPeer_; }
    bool Retain() const { return retain_; }
    const std::string& CaCert() const { return caCert_; }
    const std::string& ClientCert() const { return clientCert_; }
    const std::string& ClientKey() const { return clientKey_; }
    const std::string& SubscribeTopic() const { return subscribeTopic_; }
    const std::string& PublishTopic() const { return publishTopic_; }

    void ChangeSubscribeTopic(const std::string& newTopic) {
        subscribeTopic_ = newTopic;
    }

    void ChangePublishTopic(const std::string& newTopic) {
        publishTopic_ = newTopic;
    }

    void LoadFromTable(const toml::table &tbl) override;
    void WriteToLog(std::string_view name) override;

private:
    uint16_t port_{1883};
    uint8_t qos_ {0};
    bool enableTls_{false};
    bool verifyPeer_{true};
    bool retain_{false};
    std::string host_;
    std::string clientId_;
    std::string username_;
    std::string password_;
    std::string subscribeTopic_;
    std::string publishTopic_;
    std::string caCert_;
    std::string clientCert_;
    std::string clientKey_;
};

class TransportConfig : public IConfig {
public:
    TransportConfig() = default;
    ~TransportConfig() override = default;

    const std::string& Type() const { return type_; }
    const std::string& NodeName() const { return nodeName_; }
    const std::string& PublishTopic() const { return publishTopic_; }
    const std::string& SubscribeTopic() const { return subscribeTopic_; }

    void LoadFromTable(const toml::table &tbl) override;
    void WriteToLog(std::string_view name) override;

private:
    std::string type_;
    std::string nodeName_;
    std::string publishTopic_;
    std::string subscribeTopic_;
    uint16_t bufferSize_{4096};
};

class Config {
public:
    Config() = default;
    explicit Config(const std::filesystem::path& filePath) {
        LoadFromToml(filePath.string());
    }
    ~Config() = default;

    const LogConfig& Log() const { return logConfig_; }
    const MqttConfig& Mqtt() const { return mqttConfig_; }
    const TransportConfig& Transport() const { return transportConfig_; }

    void ChangeSubscribeTopic(const std::string& newTopic) {
        mqttConfig_.ChangeSubscribeTopic(newTopic);
    }

    void ChangePublishTopic(const std::string& newTopic) {
        mqttConfig_.ChangePublishTopic(newTopic);
    }

    void LoadFromToml(const std::string& filePath);

private:
    LogConfig logConfig_;
    MqttConfig mqttConfig_;
    TransportConfig transportConfig_;
};

}  // namespace MqttBridge