//
// Created by edo on 19/12/23.
//

#include "rtype/systems/shared/MonsterDie.hpp"

namespace rtype
{
    MonsterDie::MonsterDie(aecs::World &world,
                                 const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
        ALogicSystem(world, entities, {typeid(MonsterComponent), typeid(PositionComponent)})
    {
    }

    aecs::EntityChanges MonsterDie::update(aecs::UpdateParams &updateParams)
    {
        for (auto &[_id, entity] : _entitiesMap) {
            auto &position = entity->getComponent<PositionComponent>();
            if (position.x < -100 || position.x > 1920 + 100 || position.y < -200 || position.y > 1080 + 200) {
                updateParams.entityChanges.deletedEntities.insert(entity->getId());
            }
        }
        return updateParams.entityChanges;
    }

} // namespace rtype
