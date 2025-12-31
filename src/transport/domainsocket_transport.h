#pragma once

#include <string_view>

#include "mqttbridge/itransport.h"
#include "mqttbridge/config.h"
#include "domainsocket/server.h"
#include "domainsocket/client.h"
#include "mqttbridge/blockingqueue.h"

namespace MqttBridge {

class DomainSocketTransport : public ITransport {
public:
    DomainSocketTransport(TransportConfig config);
    DomainSocketTransport(TransportConfig config,
                          MessageCallback cb);
    ~DomainSocketTransport() override = default;

    bool Start() override;
    void Stop() override;
    void SetMessageCallback(MessageCallback callback) override;
    bool Send(const Message& message) override;

private:
    DomainSocket::Server server_;
    DomainSocket::Client client_;
};

}
