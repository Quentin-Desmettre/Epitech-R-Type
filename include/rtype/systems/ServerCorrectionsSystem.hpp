//
// Created by qdesmettre on 20/12/23.
//

#ifndef R_TYPE_SERVERCORRECTIONSSYSTEM_HPP
#define R_TYPE_SERVERCORRECTIONSSYSTEM_HPP

#include "aecs/StaticPacketParser.hpp"
#include "aecs/SystemBase.hpp"
#include "aecs/World.hpp"
#include "rtype/NetworkGlobals.hpp"
#include "rtype/components/NetworkTagComponent.hpp"
#include "rtype/components/PositionComponent.hpp"
#include <iostream>
#include <map>

namespace rtype
{
    class ServerCorrectionsSystem : public aecs::ALogicSystem
    {
      public:
        ServerCorrectionsSystem(aecs::World &world,
                                const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
            ALogicSystem(world, entities, {typeid(ClientAdressComponent)})
        {
            _socket.bind(SERVER_CORRECTIONS_PORT);
            _socket.setBlocking(false);
        }

        ~ServerCorrectionsSystem() override = default;

        aecs::EntityChanges update(aecs::UpdateParams &updateParams) override
        {
            // Send edited / deleted entities
            const auto &editedEntityIds = updateParams.entityChanges.editedEntities;
            const auto &deletedEntityIds = updateParams.entityChanges.deletedEntities;
            std::vector<aecs::EntityPtr> editedEntities, deletedEntities;
            aecs::World::EncodedGameState state;

            // Push edited entities in the game state
            for (auto &editedEntityId : editedEntityIds) {
                const auto &entity = _world.getEntity(editedEntityId);
                if (!entity)
                    continue;
                state.encodedEntities[editedEntityId] = entity->encode();
            }

            // Push deleted entities in the game state
            for (auto &deletedEntityId : deletedEntityIds) {
                const auto &entity = _world.getEntity(deletedEntityId);
                if (!entity)
                    continue;
                state.encodedEntities[deletedEntityId] = {}; // Empty encoded entity means it's deleted
            }

            // Notify clients
            sf::Packet packet = aecs::StaticPacketBuilder::buildGameChangesPacket({state});
            sendDataToClients(packet);

            return {};
        }

      private:
        void sendDataToClients(sf::Packet data)
        {
            for (auto &[_, client] : _entitiesMap) {
                auto &address = client->getComponent<ClientAdressComponent>().adress;
                sf::Socket::Status status = _socket.send(data, sf::IpAddress(address), CLIENT_CORRECTIONS_PORT);

                if (status != sf::Socket::Done)
                    std::cerr << "Error while sending data to client" << std::endl;
            }
        }

        sf::UdpSocket _socket;
    };
} // namespace rtype

#endif // R_TYPE_SERVERCORRECTIONSSYSTEM_HPP
