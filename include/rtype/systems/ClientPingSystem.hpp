//
// Created by qdesmettre on 20/12/23.
//

#ifndef R_TYPE_CLIENTPINGSYSTEM_HPP
#define R_TYPE_CLIENTPINGSYSTEM_HPP

#include "aecs/SystemBase.hpp"
#include "aecs/World.hpp"
#include "rtype/NetworkGlobals.hpp"
#include "rtype/StaticPacketParser.hpp"
#include "rtype/components/ClientPingComponent.hpp"
#include <SFML/Network.hpp>

namespace rtype
{

    class ClientPingSystem : public aecs::ALogicSystem
    {
      public:
        ClientPingSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
            ALogicSystem(world, entities, {typeid(ClientPingComponent)})
        {
            _socket.setBlocking(false);
        }

        ~ClientPingSystem() override = default;

        void update(const aecs::UpdateParams &updateParams) override
        {
            for (auto &[_, entity] : _entitiesMap) {
                auto &component = entity->getComponent<ClientPingComponent>();
                if (component.clock.getElapsedTime().asMilliseconds() < 1000)
                    continue;

                sf::Packet packet;
                packet << static_cast<sf::Uint16>(1) << PacketTypes::PING;
                _socket.send(packet, "127.0.0.1", SERVER_INPUTS_PORT); // TODO: get from ac/av

                component.clock.restart();
            }
        }

      private:
        sf::UdpSocket _socket;
    };
} // namespace rtype

#endif // R_TYPE_CLIENTPINGSYSTEM_HPP
