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
        void update(const aecs::UpdateParams &updateParams) override
        { // TODO: take as parameter a SystemParams object
            if (_connected)
                return;

            // Connect to server
            sf::Socket::Status status = _socket.connect("127.0.0.1", SERVER_TCP_PORT); // TODO: get from ac/av
            if (status != sf::Socket::Done) {
                std::cerr << "Error connecting to server" << std::endl;
                return;
            }

            // Wait for message
            sf::Packet packet;
            status = _socket.receive(packet);
            _socket.disconnect(); // disconnect anyway
            if (status != sf::Socket::Done) {
                std::cerr << "Error receiving packet" << std::endl;
                return;
            }

            // Set game state
            // _world.load(packet.getData(), packet.getDataSize());

            // Create myself (to activate udp systems)
            auto &myself = _world.createEntity();
            myself.addComponent<ClientPingComponent>();

            _connected = true;
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
