//
// Created by qdesmettre on 04/01/24.
//

#include "MapSystem.hpp"
#include "aecs/World.hpp"
#include "rtype/EntityFactory.hpp"

rtype::MapSystem::MapSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities):
    ALogicSystem(world, entities, {})
{
    for (float x = 0; x < 1044; x += 36) {
        EntityFactory::createBlock(
                {x, 0},
                {36, 36},
                "assets/sprites/Monster.png",
                {-5, 0}
        );
        EntityFactory::createBlock(
                {x, 640 - 36},
                {36, 36},
                "assets/sprites/Monster.png",
                {-5, 0}
        );
    }
}

aecs::EntityChanges rtype::MapSystem::update(aecs::UpdateParams &updateParams)
{
    // world is 128 ticks per seconds
    // each seconds, blocks move 5 pixels
    // so each tick, blocks move 5 / 128 pixels or 0.0390625  pixels
    // so they will move by 36 pixels every 128 / 5 * 36 ticks, or 460 ticks
    if (_world.getTick() % 50 != 0)
        return {};
    EntityFactory::createBlock(
            {1044, 0},
            {36, 36},
            "assets/sprites/Monster.png",
            {-5, 0}
    );
    EntityFactory::createBlock(
            {1044, 640 - 36},
            {36, 36},
            "assets/sprites/Monster.png",
            {-5, 0}
    );
    return {};
}
