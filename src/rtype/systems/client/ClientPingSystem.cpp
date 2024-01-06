//
// Created by qdesmettre on 20/12/23.
//

#include "rtype/systems/client/ClientPingSystem.hpp"
#include "aecs/StaticPacketBuilder.hpp"

namespace rtype
{
    ClientPingSystem::ClientPingSystem(aecs::World &world,
                                       const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
        ALogicSystem(world, entities, {typeid(ClientPingComponent)})
    {
        _socket.setBlocking(false);
    }

    aecs::EntityChanges ClientPingSystem::update(aecs::UpdateParams &updateParams)
    {
        (void)updateParams;
        for (auto &[_, entity] : _entitiesMap) {
            auto &component = entity->getComponent<ClientPingComponent>();
            if (component.clock.getElapsedTime().asMilliseconds() < 1000)
                continue;

            sf::Packet packet = aecs::StaticPacketBuilder::buildPingPacket();
            _socket.send(packet, _world.getIp(), _world.getServerPort());

            component.clock.restart();
        }
        return {};
    }

} // namespace rtype
