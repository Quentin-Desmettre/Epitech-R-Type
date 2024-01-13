//
// Created by qdesmettre on 20/12/23.
//

#include "ClientPingSystem.hpp"
#include "StaticPacketBuilder.hpp"

ClientPingSystem::ClientPingSystem(aecs::World &world,
                                   const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
        ALogicSystem(world, entities, {typeid(ClientPingComponent)})
{
    _socket.setBlocking(false);
}

aecs::EntityChanges ClientPingSystem::update(aecs::UpdateParams &updateParams)
{
    (void) updateParams;
    for (auto &[_, entity]: _entitiesMap) {
        auto &component = entity->getComponent<ClientPingComponent>();
        if (component.clock.getElapsedTime().asMilliseconds() < 1000)
            continue;

        sf::Packet packet = aecs::StaticPacketBuilder::buildPingPacket();
        _socket.send(packet, _world.getIp(), SERVER_INPUTS_PORT); // TODO: get from ac/av

        component.clock.restart();
    }
    return {};
}
