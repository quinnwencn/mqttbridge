#include "domainsocket_transport.h"
#include "mqttbridge/logger.h"

namespace MqttBridge {

DomainSocketTransport::DomainSocketTransport(TransportConfig config) : 
    server_(config.SubscribeTopic(), config.BufferSize()), 
    client_(config.PublishTopic()) {
        LOG_TRACE("Transport Node subscribe to : {}", config.SubscribeTopic());
    }

DomainSocketTransport::DomainSocketTransport(TransportConfig config, MessageCallback cb) : 
    server_(config.SubscribeTopic(), config.BufferSize(), cb), 
    client_(config.PublishTopic()) {
        LOG_TRACE("Transport Node subscribe to : {}", config.SubscribeTopic());
    }

bool DomainSocketTransport::Start() {
    return server_.Start();
}

void DomainSocketTransport::Stop() {
    server_.Stop();
}

void DomainSocketTransport::SetMessageCallback(MessageCallback callback) {
    server_.SetMessageCallback(callback);
}

bool DomainSocketTransport::Send(const Message& message) {
    if (!client_.Connect()) {
        return false;
    }

    return client_.SendMsg(message.json);
}

}
