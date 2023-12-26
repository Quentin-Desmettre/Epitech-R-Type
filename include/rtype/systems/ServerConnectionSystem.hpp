//
// Created by qdesmettre on 20/12/23.
//

#ifndef R_TYPE_SERVERCONNECTIONSYSTEM_HPP
#define R_TYPE_SERVERCONNECTIONSYSTEM_HPP

#include "aecs/SystemBase.hpp"
#include "aecs/World.hpp"
#include "rtype/NetworkGlobals.hpp"
#include "rtype/components/MyPlayerComponent.hpp"
#include "rtype/components/PositionComponent.hpp"
#include <SFML/Network.hpp>
#include <iostream>

namespace rtype
{
    class ServerConnectionSystem : public aecs::ALogicSystem
    {
      public:
        ServerConnectionSystem(aecs::World &world,
                               const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
            ALogicSystem(world, entities, {}),
            _connected(false)
        {
        }

        ~ServerConnectionSystem() override = default;

        /*
         * 1. Connect to server via TCP socket
         * 2. Wait for message, containing game state
         * 3. set game state
         */
        aecs::EntityChanges update(aecs::UpdateParams &updateParams) override
        {
            if (_connected)
                return {};

            // Connect to server
            sf::Socket::Status status = _socket.connect("127.0.0.1", SERVER_TCP_PORT); // TODO: get from ac/av
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
            aecs::StaticPacketParser::SystemData data{
                .world = _world,
                .clientId = 0,
                ._entitiesMap = _entitiesMap,
            };
            auto parsed = aecs::StaticPacketParser::parsePacket(packet, data);
            if (parsed.type != aecs::PacketTypes::CONNECTED) {
                std::cerr << "Error parsing packet" << std::endl;
                return {};
            }
            _world.load(parsed.entityChanges[0]);
            _world.setClientId(parsed.clientId);

            // Create myself (to activate udp systems)
            _world.createEntity().addComponent<ClientPingComponent>();

            _connected = true;
            return {};
        }

        [[nodiscard]] bool isConnected() const
        {
            return _connected;
        }

      private:
        bool _connected;
        sf::TcpSocket _socket;
    };
} // namespace rtype

#endif // R_TYPE_SERVERCONNECTIONSYSTEM_HPP
