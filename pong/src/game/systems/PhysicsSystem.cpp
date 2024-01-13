//
// Created by qdesmettre on 05/12/23.
//

#include "PhysicsSystem.hpp"

PhysicsSystem::PhysicsSystem(aecs::World &world,
                             const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
        ALogicSystem(world, entities, {typeid(PositionComponent), typeid(VelocityComponent), typeid(SpriteComponent)})
{
}

aecs::EntityChanges PhysicsSystem::update(aecs::UpdateParams &updateParams)
{
    aecs::EntityChanges changes;
    for (auto &[_id, entity]: _entitiesMap) {
        auto &position = entity->getComponent<PositionComponent>();
        auto &velocity = entity->getComponent<VelocityComponent>();


        position.x += velocity.x * updateParams.deltaTime;
        position.y += velocity.y * updateParams.deltaTime;

        if (position.y < 0)
            position.y = 0;
        if (position.y > 640 - 160)
            position.y = 640 - 160;

        if (velocity.x != 0 || velocity.y != 0)
            changes.editedEntities.push_back(entity->getId());
    }
    return changes;
}
