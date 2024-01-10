//
// Created by qdesmettre on 20/12/23.
//

#include "rtype/systems/server/DeleteClientSystem.hpp"
#include "rtype/components/ClientAdressComponent.hpp"
#include "rtype/components/ClientPingComponent.hpp"

namespace rtype
{
    DeleteClientSystem::DeleteClientSystem(aecs::World &world,
                                           const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
        ALogicSystem(world, entities, {typeid(ClientAdressComponent), typeid(ClientPingComponent)})
    {
    }

    aecs::EntityChanges DeleteClientSystem::update(aecs::UpdateParams &updateParams)
    {
        for (auto &[id, entity] : _entitiesMap) {
            auto &clientAdress = entity->getComponent<ClientAdressComponent>();
            sf::Clock &clock = entity->getComponent<ClientPingComponent>().clock;
            if (clientAdress.adress == 0 || clock.getElapsedTime().asSeconds() >= 5) {
                std::cout << "Client disconnected after " << clock.getElapsedTime().asSeconds() << " seconds"
                          << std::endl;
                updateParams.entityChanges.deletedEntities.insert(id);
            }
        }
        return {};
    }
} // namespace rtype
