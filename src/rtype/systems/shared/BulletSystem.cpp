//
// Created by edo on 10/12/23.
//

#include "rtype/systems/shared/BulletSystem.hpp"
#include <cmath>
namespace rtype
{
    BulletSystem::BulletSystem(aecs::World &world,
                               const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
        ALogicSystem(world, entities, {typeid(PositionComponent), typeid(BulletComponent), typeid(VelocityComponent), typeid(SpriteComponent)})
    {
    }

    aecs::EntityChanges BulletSystem::update(aecs::UpdateParams &updateParams)
    {
        (void)updateParams;
        std::vector<std::shared_ptr<aecs::Entity>> entities;
        aecs::EntityChanges changes;

        entities.reserve(_entitiesMap.size());
        for (auto &[_id, entity] : _entitiesMap) {
            entities.push_back(entity);
        }

        for (auto &[_id, entity] : _entitiesMap) {
            auto &position = entity->getComponent<PositionComponent>();
            auto &velocity = entity->getComponent<VelocityComponent>();
            auto &sprite = entity->getComponent<SpriteComponent>();
            if ((position.x < -100 || position.x > 1920 + 100 || position.y < -100 || position.y > 1080 + 100) && _world.getIsServer()) {
                changes.deletedEntities.insert(entity->getId());
            }
            float rotation = atan2f(velocity.y, velocity.x) * 180 / static_cast<float>(M_PI);
            sprite.sprite.setRotation(rotation);
            changes.editedEntities.insert(entity->getId());
        }
        return changes;
    }

} // namespace rtype
