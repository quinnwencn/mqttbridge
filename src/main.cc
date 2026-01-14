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

    // Optionally change topics at runtime
    // config.ChangePublishTopic("new/publish/topic");
    // config.ChangeSubscribeTopic("new/subscribe/topic");

    MqttBridge::MqttBridge bridge(config);

    // Start the bridge in current thread
    // if (!bridge.Start()) {
    //     LOG_FATAL("Failed to start MQTT Bridge");
    //     return 1;
    // }

    // Or start the bridge loop in a separate thread, this will keep the main thread busy and will not execute any code after this line until Stop is called
    bridge.StartLoop();

    // Keep the main thread alive while the bridge is running
    std::this_thread::sleep_for(std::chrono::seconds(10));
    
    // Stop the bridge after some time or based on some condition
    bridge.Stop();

    LOG_TRACE("MQTT Bridge exiting...");
    return 0;
}
