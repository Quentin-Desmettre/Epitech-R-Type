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

    aecs::EntityChanges ClientPingSystem::update(unused aecs::UpdateParams &updateParams)
    {
        for (auto &[_, entity] : _entitiesMap) {
            auto &component = entity->getComponent<ClientPingComponent>();
            if (component.clock.getElapsedTime().asMilliseconds() < 1000)
                continue;

            sf::Packet packet = aecs::StaticPacketBuilder::buildPingPacket();
            _socket.send(packet, "127.0.0.1", SERVER_INPUTS_PORT); // TODO: get from ac/av

            component.clock.restart();
        }
        return {};
    }

} // namespace rtype
