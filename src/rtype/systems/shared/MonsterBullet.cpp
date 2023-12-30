//
// Created by edo on 19/12/23.
//

#include "rtype/systems/shared/MonsterBullet.hpp"

namespace rtype
{
    MonsterBullet::MonsterBullet(aecs::World &world,
                                 const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
        ALogicSystem(world, entities, {typeid(MonsterComponent), typeid(PositionComponent)})
    {
    }

    aecs::EntityChanges MonsterBullet::update(aecs::UpdateParams &updateParams)
    {
        aecs::EntityChanges changes;
        for (auto &[_id, entity] : _entitiesMap) {
            auto &monster = entity->getComponent<MonsterComponent>();
            auto &position = entity->getComponent<PositionComponent>();
            if (position.x < -100 || position.x > 1920 + 100 || position.y < -100 || position.y > 1080 + 100) {
                changes.deletedEntities.push_back(entity->getId());
            }
            if (monster._lil)
                continue;
            monster.timeSinceLastShoot += updateParams.deltaTime;
            changes.editedEntities.push_back(entity->getId());

            if (monster.timeSinceLastShoot > 15) {
                monster.timeSinceLastShoot = 0;
                EntityFactory::createBullet(sf::Vector2f(position.x - 10, position.y + 10), sf::Vector2f(-50, 0), 1,
                                            false);
            }
        }
        return changes;
    }

} // namespace rtype
