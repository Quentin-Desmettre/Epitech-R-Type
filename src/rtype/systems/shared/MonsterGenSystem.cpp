//
// Created by edo on 19/12/23.
//

#include "rtype/systems/shared/MonsterGenSystem.hpp"
#include "rtype/components/DifficultyComponent.hpp"
#include <cmath>

rtype::MonsterGenSystem::MonsterGenSystem(aecs::World &world,
                                          const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
    ALogicSystem(world, entities, {typeid(PlayerComponent)})
{
}

aecs::Entity &rtype::MonsterGenSystem::addRandomEnemy()
{
    int rnd = std::rand() % 100;
    bool lil = rnd < 70;

    return EntityFactory::createEnemy(sf::Vector2f(1180, std::rand() % 540 + 50), sf::Vector2f(-15, 0), lil);
}

aecs::EntityChanges rtype::MonsterGenSystem::update(aecs::UpdateParams &updateParams)
{
    std::size_t nbPlayer = _entitiesMap.size();
    static float time = 0;
    aecs::EntityChanges changes;
    static float bossTime = 300.f;
    static std::size_t nbPlayerLast = 0;

    if (nbPlayer == 0) {
        return {};
    }
    if (nbPlayer != nbPlayerLast)
        bossTime = 300.f;
    nbPlayerLast = nbPlayer;

    time += updateParams.deltaTime;
    bossTime -= updateParams.deltaTime;

    int rnd;
    float difficulty = EntityFactory::getDifficulty();
    while (time > 1) {
        rnd = std::rand() % 100;
        if (rnd * nbPlayer < 10 * (1 + difficulty / 10.f)) {
            changes.editedEntities.insert(addRandomEnemy().getId());
        }
        if (bossTime < 0) {
            if (rand() % 2)
                EntityFactory::createSnake(sf::Vector2f(1180, std::rand() % 540 + 50), 25);
            else
                EntityFactory::createBossEnemy(sf::Vector2f(1380, std::rand() % 540 + 50), sf::Vector2f(-10, 0));
            bossTime = 600.f;
        }
        time -= 1;
    }
    return changes;
}