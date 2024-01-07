//
// Created by Clément Vandeville on 03/01/2024.
//

#include "rtype/systems/client/ClientScoreSystem.hpp"
#include "rtype/components/HPComponent.hpp"
#include <iostream>

namespace rtype {
    ClientScoreSystem::ClientScoreSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
            ALogicSystem(world, entities, {typeid(MonsterComponent)})
    {

    }

    aecs::EntityChanges ClientScoreSystem::update(aecs::UpdateParams &updateParams)
    {
        (void)updateParams;
        return {};
    }

    void ClientScoreSystem::onEntityRemoved(const aecs::EntityPtr &entity)
    {
        if (!entity->hasComponent<MonsterComponent>())
            return;

        auto &monster = entity->getComponent<MonsterComponent>();
        if (monster._lil)
            _score += 1;
        else
            _score += 10;
    }
}