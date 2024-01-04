//
// Created by qdesmettre on 20/12/23.
//

#include "rtype/systems/client/ClientInputSenderSystem.hpp"

namespace rtype
{

    ClientInputSenderSystem::ClientInputSenderSystem(
        aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
        ALogicSystem(world, entities, {typeid(ClientPingComponent)})
    {
        _socket.setBlocking(false);
    }

    /*
     * send inputs to server, via UDP
     */
    aecs::EntityChanges ClientInputSenderSystem::update(aecs::UpdateParams &updateParams)
    {
        if (updateParams.inputs.empty() || updateParams.inputs.begin()->second.empty())
            return {};

        // Get the first inputs, as a client only has one player
        std::vector<aecs::RenderInput> inputs = updateParams.inputs.begin()->second;
        sf::Packet packet = aecs::StaticPacketBuilder::buildGameInputPacket(inputs);
        _socket.send(packet, _world.getIp(), _world.getServerPort()); // TODO: get from ac/av
        for (auto &[_, entity] : _entitiesMap)
            entity->getComponent<ClientPingComponent>().clock.restart();
        return {};
    }

} // namespace rtype
