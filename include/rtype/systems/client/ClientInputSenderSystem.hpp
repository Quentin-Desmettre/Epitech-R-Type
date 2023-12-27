//
// Created by qdesmettre on 20/12/23.
//

#ifndef R_TYPE_CLIENTINPUTSENDERSYSTEM_HPP
#define R_TYPE_CLIENTINPUTSENDERSYSTEM_HPP

#include "SFML/Network.hpp"
#include "aecs/StaticPacketBuilder.hpp"
#include "aecs/StaticPacketParser.hpp"
#include "aecs/SystemBase.hpp"
#include "aecs/World.hpp"
#include "rtype/NetworkGlobals.hpp"
#include "rtype/components/ClientPingComponent.hpp"

namespace rtype
{

    /**
     * @brief Sends inputs to server
     */
    class ClientInputSenderSystem : public aecs::ALogicSystem
    {
      public:
        ClientInputSenderSystem(aecs::World &world,
                                const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
            ALogicSystem(world, entities, {typeid(ClientPingComponent)})
        {
            _socket.setBlocking(false);
            _socket.bind(CLIENT_INPUTS_PORT);
        }

        ~ClientInputSenderSystem() override = default;

        /*
         * send inputs to server, via UDP
         */
        aecs::EntityChanges update(aecs::UpdateParams &updateParams) override
        {
            if (updateParams.inputs.empty() || updateParams.inputs.begin()->second.empty())
                return {};

            // Get the first inputs, as a client only has one player
            std::vector<aecs::RenderInput> inputs = updateParams.inputs.begin()->second;
            sf::Packet packet = aecs::StaticPacketBuilder::buildGameInputPacket(inputs);
            _socket.send(packet, "127.0.0.1", SERVER_INPUTS_PORT); // TODO: get from ac/av
            for (auto &[_, entity] : _entitiesMap)
                entity->getComponent<ClientPingComponent>().clock.restart();
            return {};
        }

      private:
        sf::UdpSocket _socket;
    };
} // namespace rtype

#endif // R_TYPE_CLIENTINPUTSENDERSYSTEM_HPP
