#include "mqttbridge/config.h"
#include "mqttbridge/logger.h"

namespace MqttBridge {

template <typename T>
void WriteToLog(T& t, std::string_view name) {
    t.WriteToLog(name);
}

void LogConfig::LoadFromTable(const toml::table &tbl) {
    logDirs_ = tbl.at("log_dir").value_or("");
    logLevel_ = tbl.at("level").value_or(2);
}

void LogConfig::WriteToLog(std::string_view name) {
    LOG_TRACE("[{}]", name);
    LOG_TRACE("log_dir = {}", logDirs_.string());
    LOG_TRACE("level = {}", logLevel_);
}

void MqttConfig::LoadFromTable(const toml::table &tbl) {
    enableTls_ = tbl.at("enable_tls").value_or(true);
    verifyPeer_ = tbl.at("verify_peer").value_or(true);
    retain_ = tbl.at("retain").value_or(false);
    host_ = tbl.at("host").value_or("");
    port_ = tbl.at("port").value_or(1883);
    qos_ = tbl.at("qos").value_or(0);
    clientId_ = tbl.at("client_id").value_or("");
    if (tbl.contains("username")) {
        username_ = tbl.at("username").value_or("");
        password_ = tbl.at("password").value_or("");
    }

    subscribeTopic_ = tbl.at("subscribe_topic").value_or("");
    publishTopic_ = tbl.at("publish_topic").value_or("");
    if (enableTls_) {
        caCert_ = tbl.at("ca_cert").value_or("");
        clientCert_ = tbl.at("client_cert").value_or("");
        clientKey_ = tbl.at("client_key").value_or("");
    } else {
        caCert_.clear();
        clientCert_.clear();
        clientKey_.clear();
    }
}

void MqttConfig::WriteToLog(std::string_view name) {
    LOG_TRACE("[{}]", name);
    LOG_TRACE("host = {}", host_);
    LOG_TRACE("port = {}", port_);
    LOG_TRACE("client_id = {}", clientId_);
    LOG_TRACE("username = {}", username_);
    LOG_TRACE("password = {}", password_);
    LOG_TRACE("subscribe_topic = {}", subscribeTopic_);
    LOG_TRACE("publish_topic = {}", publishTopic_);
    LOG_TRACE("enable_tls = {}", enableTls_);
    LOG_DEBUG("verify_peer = {}", verifyPeer_);
    LOG_TRACE("ca_cert = {}", caCert_);
    LOG_TRACE("client_cert = {}", clientCert_);
    LOG_TRACE("client_key = {}", clientKey_);
}

void TransportConfig::LoadFromTable(const toml::table &tbl) {
    type_ = tbl.at("type").value_or("");
    nodeName_ = tbl.at("node_name").value_or("");
    publishTopic_ = tbl.at("publish_topic").value_or("");
    subscribeTopic_ = tbl.at("subscribe_topic").value_or("");
}

void TransportConfig::WriteToLog(std::string_view name) {
    LOG_TRACE("[{}]", name);
    LOG_TRACE("type = {}", type_);
    LOG_TRACE("node_name = {}", nodeName_);
    LOG_TRACE("publish_topic = {}", publishTopic_);
    LOG_TRACE("subscribe_topic = {}", subscribeTopic_);
}

void Config::LoadFromToml(const std::string& filePath) {
     if (!std::filesystem::exists(filePath)) {
        throw std::runtime_error("Config file does not exist");
    }

    auto tbl = toml::parse_file(filePath);
    transportConfig_.LoadFromTable(*tbl.at("transport").as_table());
    mqttConfig_.LoadFromTable(*tbl.at("mqtt").as_table());
    logConfig_.LoadFromTable(*tbl.at("log").as_table());
    
    Logger::Init(logConfig_);
}

}  // namespace MqttBridge
