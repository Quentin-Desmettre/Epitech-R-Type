//
// Created by edo on 10/12/23.
//

#include "rtype/systems/shared/BulletSystem.hpp"

namespace rtype
{
    BulletSystem::BulletSystem(aecs::World &world,
                               const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
        ALogicSystem(world, entities, {typeid(PositionComponent), typeid(BulletComponent)})
    {
    }

    aecs::EntityChanges BulletSystem::update(unused aecs::UpdateParams &updateParams)
    {
        std::vector<std::shared_ptr<aecs::Entity>> entities;
        aecs::EntityChanges changes;

        entities.reserve(_entitiesMap.size());
        for (auto &[_id, entity] : _entitiesMap) {
            entities.push_back(entity);
        }

        for (size_t i = 0; i < entities.size(); i++) {
            auto &entity = entities[i];
            auto &position = entity->getComponent<PositionComponent>();
            if (position.x < -100 || position.x > 1920 + 100 || position.y < -100 || position.y > 1080 + 100) {
                //                    _world.destroyEntity(*entity);
                entities.erase(entities.begin() + i);
                changes.deletedEntities.push_back(entity->getId());
            }
        }
        return changes;
    }

} // namespace rtype
