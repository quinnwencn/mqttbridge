#pragma once

#include <string>

namespace MqttBridge {

struct Message {
        std::string topic;
        std::string json;
};

}