//
// Created by qdesmettre on 20/12/23.
//

#include "ServerConnectionSystem.hpp"

ServerConnectionSystem::ServerConnectionSystem(
        aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
        ALogicSystem(world, entities, {}),
        _connected(false)
{
}

/*
 * 1. Connect to server via TCP socket
 * 2. Wait for message, containing game state
 * 3. set game state
 */
aecs::EntityChanges ServerConnectionSystem::update(aecs::UpdateParams &updateParams)
{
    (void) updateParams;
    if (_connected)
        return {};

    // Connect to server
    sf::Socket::Status status = _socket.connect(_world.getIp(), _world.getTcpPort(), sf::seconds(5));
    if (status != sf::Socket::Done) {
        std::cerr << "Error connecting to server" << std::endl;
        return {};
    }

    // Wait for message
    sf::Packet packet;
    status = _socket.receive(packet);
    _socket.disconnect(); // disconnect anyway
    if (status != sf::Socket::Done) {
        std::cerr << "Error receiving packet" << std::endl;
        return {};
    }

    // Parse connected answer
    auto parsed = aecs::StaticPacketParser::parsePacket(packet, 0);
    if (parsed.type != aecs::PacketTypes::CONNECTED) {
        std::cerr << "Error parsing packet" << std::endl;
        return {};
    }
    for (auto &entity: parsed.entityChanges)
        _world.load(entity);
    _world.setClientId(parsed.clientId);

    // Create myself (to activate udp systems)
    _world.createEntity().addComponent<ClientPingComponent>();

    _connected = true;
    return {};
}

bool ServerConnectionSystem::isConnected() const
{
    return _connected;
}
