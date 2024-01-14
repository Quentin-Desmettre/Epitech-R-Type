//
// Created by qdesmettre on 20/12/23.
//

#include "DeleteClientSystem.hpp"
#include "ClientAddressComponent.hpp"
#include "ClientPingComponent.hpp"

DeleteClientSystem::DeleteClientSystem(aecs::World &world,
                                       const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
        ALogicSystem(world, entities, {typeid(ClientAddressComponent), typeid(ClientPingComponent)})
{
}

aecs::EntityChanges DeleteClientSystem::update(aecs::UpdateParams &updateParams)
{
    for (auto &[id, entity]: _entitiesMap) {
        auto &clientAdress = entity->getComponent<ClientAddressComponent>();
        sf::Clock &clock = entity->getComponent<ClientPingComponent>().clock;
        if (clientAdress.address == 0 || clock.getElapsedTime().asSeconds() >= 5) {
            std::cout << "Client disconnected after " << clock.getElapsedTime().asSeconds() << " seconds"
                      << std::endl;
            _world.playerConnected--;
            updateParams.entityChanges.deletedEntities.insert(id);
        }
    }
    return {};
}
