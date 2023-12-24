//
// Created by qdesmettre on 20/12/23.
//

#ifndef R_TYPE_CLIENTPINGSYSTEM_HPP
#define R_TYPE_CLIENTPINGSYSTEM_HPP

#include "aecs/SystemBase.hpp"
#include "aecs/World.hpp"
#include "rtype/NetworkGlobals.hpp"
#include "rtype/StaticPacketParser.hpp"
#include <SFML/Network.hpp>

namespace rtype
{

    class ClientPingSystem : public aecs::ALogicSystem
    {
      public:
        ClientPingSystem(aecs::World &world,
                                const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
            ALogicSystem(world, entities, {})
        {
            _socket.setBlocking(false);
        }

        ~ClientPingSystem() override = default;

        void update(const aecs::UpdateParams &updateParams) override
        {
            sf::Packet packet;

            if (_world.getTimeSinceLastCommunication() < 1000)
                return;

            packet << static_cast<sf::Uint16>(1) << PacketTypes::PING;

            _socket.send(packet, "127.0.0.1", SERVER_UDP_PORT); // TODO: get from ac/av
            _world.resetTimeSinceLastCommunication();
        }

      private:
        sf::UdpSocket _socket;
    };
} // namespace rtype

#endif // R_TYPE_CLIENTPINGSYSTEM_HPP
