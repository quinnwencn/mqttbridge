# MQTT Bridge

A C++ MQTT Bridge library that enables communication between MQTT clients and various transport mechanisms. This project provides a flexible framework for bridging MQTT messages to different transport layers such as domain sockets.

## Features

- **MQTT Client Integration**: Built on Mosquitto MQTT library for reliable MQTT communication
- **Pluggable Transport Architecture**: Support for multiple transport types (currently domain sockets)
- **Thread-safe Message Queuing**: Blocking queues for reliable message passing between MQTT and transport layers
- **Configuration-driven**: TOML-based configuration for easy setup and deployment
- **Logging System**: Comprehensive logging with configurable levels
- **C++20 Modern**: Written in modern C++ with proper RAII and smart pointer usage

## Dependencies

- **Boost** (system, log, log_setup, filesystem)
- **tomlplusplus** - TOML configuration parsing
- **fmt** - Formatting library
- **CLI11** - Command line interface
- **mosquitto** - MQTT client library

## Building

### Prerequisites

- CMake 3.20 or higher
- C++20 compliant compiler
- Conan package manager

### Build Steps

1. Install dependencies using Conan:
```bash
conan install . --build=missing
```

2. Build the project:
```bash
cmake --preset conan-default
cmake --build --preset conan-default
```

## Usage

### Configuration

Create a TOML configuration file (example in `test/config/config.toml`):

```toml
[mqtt]
host = "localhost"
port = 1883
client_id = "example_mqtt_client"
subscribe_topic = "example/subscribe"
publish_topic = "example/publish"
enable_tls = false
verify_peer = false
qos = 1
retain = false

[log]
log_dir = "/tmp"
level = 0

[transport]
node_name = "domainsocket"
type = "domainsocket"
publish_topic = "/tmp/mqttbridge_publish.sock"
subscribe_topic = "/tmp/mqttbridge_subscribe.sock"
```

### Running the Bridge

```bash
./xbridge -c path/to/config.toml
```

### Examples

The project includes several examples:

- **Domain Socket Client/Server**: See `examples/domainsocket/`
- **MQTT Client**: See `examples/mqttclient/`
- **Transport Test**: See `examples/transport/`

Build examples by enabling them in the CMake configuration.

## Architecture

The bridge consists of several key components:

- **MqttBridge**: Main orchestrator that coordinates between MQTT client and transport
- **IMqttClient**: Interface for MQTT client implementations
- **ITransport**: Interface for transport layer implementations
- **Config**: Configuration management from TOML files
- **BlockingQueue**: Thread-safe message queues for communication
- **Logger**: Configurable logging system

## Project Structure

```
mqttbridge/
├── include/mqttbridge/     # Header files
├── src/                    # Source implementation
│   ├── bridge/            # Core bridge logic
│   ├── config/            # Configuration management
│   ├── domainsocket/      # Domain socket transport
│   ├── logger/            # Logging system
│   ├── mqttclient/        # MQTT client implementation
│   └── transport/         # Transport factory and interfaces
├── examples/              # Usage examples
├── test/                  # Test configurations
├── CMakeLists.txt         # Main build configuration
└── conanfile.py          # Dependency management
```

## Using libmqttbridge.so

The mqttbridge library can be used as a shared library (`libmqttbridge.so`) in your own applications. Below is a step-by-step guide based on the example in `src/main.cc`.

### Basic Integration

1. **Include Required Headers**:
```cpp
#include "mqttbridge/logger.h"
#include "mqttbridge/mqttbridge.h"
#include "mqttbridge/config.h"
```

2. **Load Configuration**:
```cpp
// Path to your TOML configuration file
std::filesystem::path configFilePath("path/to/config.toml");

// Create configuration object
MqttBridge::Config config(configFilePath);
```

3. **Create and Start the Bridge**:
```cpp
// Create bridge instance
MqttBridge::MqttBridge bridge(config);

// Option 1: Start in current thread (blocking)
if (!bridge.Start()) {
    LOG_FATAL("Failed to start MQTT Bridge");
    return 1;
}

// Option 2: Start bridge loop in separate thread
bridge.StartLoop();
```

4. **Keep Application Running**:
```cpp
// Keep main thread alive while bridge runs
std::this_thread::sleep_for(std::chrono::seconds(10));
```

5. **Clean Shutdown**:
```cpp
// Stop the bridge gracefully
bridge.Stop();
```

### Complete Example

Here's a complete working example based on `src/main.cc`:

```cpp
#include <CLI/CLI.hpp>
#include <filesystem>
#include <thread>
#include <chrono>

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

    // Load configuration
    MqttBridge::Config config(configFilePath);

    // Optionally change topics at runtime
    // config.ChangePublishTopic("new/publish/topic");
    // config.ChangeSubscribeTopic("new/subscribe/topic");

    // Create bridge instance
    MqttBridge::MqttBridge bridge(config);

    // Start the bridge loop in a separate thread
    bridge.StartLoop();

    // Keep the main thread alive while the bridge is running
    std::this_thread::sleep_for(std::chrono::seconds(10));
    
    // Stop the bridge after some time or based on some condition
    bridge.Stop();

    LOG_TRACE("MQTT Bridge exiting...");
    return 0;
}
```

### Compilation

When linking against `libmqttbridge.so`, ensure you:

1. Include the header directory: `-I/path/to/mqttbridge/include`
2. Link against the library: `-L/path/to/mqttbridge/lib -lmqttbridge`
3. Link all dependencies: `-lmosquitto -lboost_system -lboost_log -lboost_log_setup -lboost_filesystem -ltomlplusplus -lfmt`

Example compilation command:
```bash
g++ -std=c++20 -I./include your_app.cpp -L./build -lmqttbridge -lmosquitto -lboost_system -lboost_log -lboost_log_setup -lboost_filesystem -ltomlplusplus -lfmt -o your_app
```

### Key Points

- The bridge handles all MQTT communication and transport management internally
- Use `StartLoop()` for non-blocking operation in separate threads
- Use `Start()` for blocking operation in the current thread
- Always call `Stop()` for clean shutdown
- Configuration can be modified at runtime through the `Config` object
- The bridge uses RAII - destruction automatically cleans up resources

## License

This project is licensed under the terms specified in the project configuration.