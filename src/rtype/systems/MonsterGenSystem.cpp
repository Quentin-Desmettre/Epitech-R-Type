//
// Created by edo on 19/12/23.
//

#include "rtype/systems/MonsterGenSystem.hpp"

void rtype::MonsterGenSystem::addRandomEnemy()
{
    int rnd = std::rand() % 100;
    bool lil = rnd < 70;
    EntityFactory::createEnemy(sf::Vector2f(1080, std::rand() % 540), sf::Vector2f(-15, 0), lil);
}

void rtype::MonsterGenSystem::update(const aecs::UpdateParams &updateParams)
{
    std::size_t nbPlayer = _entitiesMap.size();
    static float time = 0;
    time += updateParams.deltaTime;

    if (nbPlayer == 0) {
        return;
    }
    int rnd = 0;

    while (time > 1) {
        rnd = std::rand() % 100;
        if (rnd * nbPlayer < 10) {
            addRandomEnemy();
        }
        time -= 1;
    }
}