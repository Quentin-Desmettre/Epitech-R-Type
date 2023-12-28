//
// Created by edo on 19/12/23.
//

#include "rtype/systems/shared/MonsterGenSystem.hpp"

rtype::MonsterGenSystem::MonsterGenSystem(aecs::World &world,
                                          const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
    ALogicSystem(world, entities, {typeid(PlayerComponent)})
{
}

aecs::Entity &rtype::MonsterGenSystem::addRandomEnemy()
{
    int rnd = std::rand() % 100;
    bool lil = rnd < 70;
    return EntityFactory::createEnemy(sf::Vector2f(1080, std::rand() % 540), sf::Vector2f(-15, 0), lil);
}

aecs::EntityChanges rtype::MonsterGenSystem::update(aecs::UpdateParams &updateParams)
{
    std::size_t nbPlayer = _entitiesMap.size();
    static float time = 0;
    time += updateParams.deltaTime;
    aecs::EntityChanges changes;

    if (nbPlayer == 0) {
        return {};
    }

    int rnd;
    while (time > 1) {
        rnd = std::rand() % 100;
        if (rnd * nbPlayer < 10) {
            changes.editedEntities.push_back(addRandomEnemy().getId());
        }
        time -= 1;
    }
    return changes;
}