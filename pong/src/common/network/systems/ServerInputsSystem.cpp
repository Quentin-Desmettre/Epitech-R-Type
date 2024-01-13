//
// Created by qdesmettre on 20/12/23.
//

#include "ServerInputsSystem.hpp"

ServerInputsSystem::ServerInputsSystem(aecs::World &world,
                                       const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
        ALogicSystem(world, entities,
                     {typeid(ClientAddressComponent), typeid(PlayerComponent), typeid(ClientPingComponent)})
{
    _socket.bind(SERVER_INPUTS_PORT);
    _socket.setBlocking(false);
}

aecs::EntityChanges ServerInputsSystem::update(aecs::UpdateParams &updateParams)
{
    sf::Packet packet;

    // Receive a packet
    while (_socket.receive(packet, _sender, _port) == sf::Socket::Done) {
        if (_entitiesMap.empty())
            continue;
        // Get player from it
        std::size_t clientId = -1;
        aecs::EntityPtr client = findClient(clientId);
        if (clientId == (std::size_t) -1) {
            std::cerr << "Unknown client \"" << _sender.toString() << "\"" << std::endl;
            continue;
        }

        aecs::StaticPacketParser::ParsedData parsed = aecs::StaticPacketParser::parsePacket(packet, clientId);
        handlePacket(parsed, updateParams, clientId);

        // Anyway, reset ping clock
        client->getComponent<ClientPingComponent>().clock.restart();
    }
    return {};
}

aecs::EntityPtr &ServerInputsSystem::findClient(std::size_t &clientId)
{
    for (auto &[id, entity]: _entitiesMap) {
        auto &clientAdress = entity->getComponent<ClientAddressComponent>();
        if (clientAdress.address == _sender.toInteger()) {
            clientId = entity->getComponent<PlayerComponent>().playerId;
            return entity;
        }
    }
    return _entitiesMap.begin()->second;
}

void ServerInputsSystem::handlePacket(aecs::StaticPacketParser::ParsedData &packet,
                                      aecs::UpdateParams &updateParams, std::size_t clientId)
{
    switch (packet.type) {
        case aecs::NONE:
            std::cerr << "Error parsing packet" << std::endl;
            break;
        case aecs::PING:
            sendPong(_sender);
            break;
        case aecs::CLIENT_PONG:
            break;
        case aecs::GAME_INPUT:
            updateParams.inputs[clientId] = packet.inputs[clientId];
            break;
        default:
            std::cerr << "Unexpected packet type" << std::endl;
            break;
    }
}

void ServerInputsSystem::sendPong(sf::IpAddress &sender)
{
    sf::Packet packet = aecs::StaticPacketBuilder::buildServerPongPacket();
    sf::Socket::Status status = _socket.send(packet, sender, CLIENT_CORRECTIONS_PORT);

    if (status != sf::Socket::Done)
        std::cerr << "Error sending packet" << std::endl;
}
