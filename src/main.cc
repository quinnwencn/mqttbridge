#include <CLI/CLI.hpp>
#include <filesystem>
#include <toml++/toml.hpp>

#include "mqttbridge/logger.h"
#include "mqttbridge/mqttbridge.h"
#include "mqttbridge/config.h"

int main(int argc, char** argv) {
    CLI::App app{"MQTT Bridge Application"};
    std::filesystem::path configFilePath;

    app.add_option("-c,--config", configFilePath, "Path to configuration TOML file")
        ->required()
        ->check(CLI::ExistingFile);
    CLI11_PARSE(app, argc, argv);

    MqttBridge::Config config(configFilePath);

    Logger::Init(config.Log());
    MqttBridge::MqttBridge bridge(config);

    if (!bridge.Start()) {
        LOG_FATAL("Failed to start MQTT Bridge");
        return 1;
    }

    LOG_TRACE("MQTT Bridge exiting...");
    return 0;
}