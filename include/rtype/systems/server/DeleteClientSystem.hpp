//
// Created by qdesmettre on 20/12/23.
//

#ifndef R_TYPE_DELETECLIENTSYSTEM_HPP
#define R_TYPE_DELETECLIENTSYSTEM_HPP

#include "aecs/SystemBase.hpp"
#include "aecs/World.hpp"
#include "rtype/components/ClientAdressComponent.hpp"
#include "rtype/components/ClientPingComponent.hpp"
#include <iostream>

namespace rtype
{
    class DeleteClientSystem : public aecs::ALogicSystem
    {
      public:
        DeleteClientSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
            ALogicSystem(world, entities, {typeid(ClientAdressComponent), typeid(ClientPingComponent)})
        {
        }

        ~DeleteClientSystem() override = default;

        aecs::EntityChanges update(aecs::UpdateParams &updateParams) override
        {
            for (auto &[id, entity] : _entitiesMap) {
                auto &clientAdress = entity->getComponent<ClientAdressComponent>();
                sf::Clock &clock = entity->getComponent<ClientPingComponent>().clock;
                if (clientAdress.adress == 0 || clock.getElapsedTime().asSeconds() >= 5) {
                    std::cout << "Client disconnected after " << clock.getElapsedTime().asSeconds() << " seconds"
                              << std::endl;
                    updateParams.entityChanges.deletedEntities.push_back(id);
                }
            }
            return {};
        }
    };
} // namespace rtype

#endif // R_TYPE_DELETECLIENTSYSTEM_HPP
