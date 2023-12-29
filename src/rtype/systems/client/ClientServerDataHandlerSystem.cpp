//
// Created by qdesmettre on 20/12/23.
//

#include "rtype/systems/client/ClientServerDataHandlerSystem.hpp"

namespace rtype
{
    ClientServerDataHandlerSystem::ClientServerDataHandlerSystem(
        aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
        ALogicSystem(world, entities, {typeid(ClientPingComponent)}),
        _tcpHandshakeSystem(world, entities),
        _maxReceivedTick(0)
    {
        _socket.bind(CLIENT_CORRECTIONS_PORT);
        _socket.setBlocking(false);
    }

    aecs::EntityChanges ClientServerDataHandlerSystem::update(aecs::UpdateParams &updateParams)
    {
        _tcpHandshakeSystem.update(updateParams);
        if (!_tcpHandshakeSystem.isConnected())
            return {};

        // Receive a packet
        sf::Packet packet;
        sf::IpAddress sender;
        unsigned short port;

        while (_socket.receive(packet, sender, port) == sf::Socket::Done) {
            // If packet:
            // - send pong
            // - check if tick has already been checked
            // - push to queue
            // - do packet received X ms in the past

            aecs::StaticPacketParser::ParsedData parsed = aecs::StaticPacketParser::parsePacket(packet, 0);

            if (parsed.type == aecs::SERVER_PONG)
                return {};
            if (parsed.type != aecs::GAME_CHANGES) {
                std::cerr << "Unexpected packet type" << std::endl;
                return {};
            }

            // Send pong with tick
            unsigned tick = parsed.tick;
            sf::Packet pongPacket = aecs::StaticPacketBuilder::buildClientPongPacket(std::max(tick, _maxReceivedTick));
            _socket.send(pongPacket, sender, SERVER_INPUTS_PORT);

            // Reset ping clock
            for (auto &[_, entity] : _entitiesMap)
                entity->getComponent<ClientPingComponent>().clock.restart();

            // Do the changes
            for (const auto &change : parsed.entityChanges) {
                _world.load(change);
            }
        }

        //            // Check if tick has already been checked
        //            if (tick <= _maxReceivedTick)
        //                return {};
        //
        //            // Push to queue
        //            auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
        //                std::chrono::system_clock::now().time_since_epoch());
        //            _maxReceivedTick = tick;
        //            _packets.emplace(packet, now);
        //
        //            // Do packet received X ms in the past
        //            if (_packets.empty())
        //                return {};
        //            auto &[firstPacket, timeReceived] = _packets.front();
        //            if (now - timeReceived < std::chrono::milliseconds(BUFFER_DELAY))
        //                return {};
        //            _packets.pop();
        //            PacketHandler::handle(_world, firstPacket);
        return {};
    }

} // namespace rtype
