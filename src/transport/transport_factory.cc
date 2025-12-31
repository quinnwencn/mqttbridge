#include "mqttbridge/itransport.h"
#include "transport/domainsocket_transport.h"

namespace MqttBridge {

    std::unique_ptr<ITransport> CreateTransport(std::string_view type, const TransportConfig& config) {
        if (type == "domainsocket") {
            return std::make_unique<DomainSocketTransport>(config);
        } else {
            throw std::invalid_argument("Unknown transport type: " + std::string(type));
        }
        // Add other transport types here as needed

        return nullptr; // Unknown transport type
    }

} // namespace MqttBridge