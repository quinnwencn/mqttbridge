#include "domainsocket_transport.h"

namespace MqttBridge {

DomainSocketTransport::DomainSocketTransport(TransportConfig config) : 
    server_(config.SubscribeTopic(), config.BufferSize()), 
    client_(config.PublishTopic()) {}

DomainSocketTransport::DomainSocketTransport(TransportConfig config, MessageCallback cb) : 
    server_(config.SubscribeTopic(), config.BufferSize(), cb), 
    client_(config.PublishTopic()) {}

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
