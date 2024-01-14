//
// Created by edo on 09/01/24.
//

#include "rtype/systems/server/BulletGenSystem.hpp"
#include "rtype/components/BulletGenComponent.hpp"
#include "rtype/components/PositionComponent.hpp"
#include "rtype/components/DifficultyComponent.hpp"

namespace rtype
{
    BulletGenSystem::BulletGenSystem(aecs::World &world,
                                     const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
        ALogicSystem(world, entities, {typeid(BulletGenComponent), typeid(PositionComponent)})
    {
    }

    aecs::EntityChanges BulletGenSystem::update(aecs::UpdateParams &updateParams)
    {
        for (auto &[id, entity] : _entitiesMap) {
            auto &bulletGen = entity->getComponent<BulletGenComponent>();
            auto &position = entity->getComponent<PositionComponent>();

            if (!bulletGen.shoot)
                continue;

            bulletGen.time += updateParams.deltaTime;

            if (bulletGen.time < bulletGen.timeBetween)
                continue;

            bulletGen.time = 0;
            sf::Vector2f pos = {position.x, position.y};
            pos.y -= bulletGen.padding * float(bulletGen.nb - 1) / 2.f;
            for (int i = 0; i < bulletGen.nb; ++i) {
                updateParams.entityChanges.editedEntities.insert(
                    EntityFactory::createBullet(pos, bulletGen.velocity, bulletGen.team, bulletGen.big).getId());
                pos.y += bulletGen.padding;
            }
        }
        return {};
    }
} // namespace rtype
