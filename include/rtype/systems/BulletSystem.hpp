//
// Created by edo on 10/12/23.
//

#ifndef R_TYPE_BULLETSYSTEM_HPP
#define R_TYPE_BULLETSYSTEM_HPP

#include "aecs/SystemBase.hpp"
#include "aecs/World.hpp"
#include "rtype/components/BulletComponent.hpp"
#include "rtype/components/PositionComponent.hpp"
#include "rtype/components/SpriteComponent.hpp"
#include "rtype/components/VelocityComponent.hpp"
#include <iostream>
namespace rtype
{

    class BulletSystem : public aecs::ALogicSystem
    {
      public:
        BulletSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
            ALogicSystem(world, entities, {typeid(PositionComponent), typeid(BulletComponent)})
        {
        }
        ~BulletSystem() override = default;

        void update(const std::vector<aecs::RenderInput> &inputs, float deltaTime) override
        {
            std::vector<std::shared_ptr<aecs::Entity>> entities;

            entities.reserve(_entitiesMap.size());
            for (auto &[_id, entity] : _entitiesMap) {
                entities.push_back(entity);
            }

            for (size_t i = 0; i < entities.size(); i++) {
                auto &entity = entities[i];
                auto &position = entity->getComponent<PositionComponent>();
                if (position.x < -100 || position.x > 1920 + 100 || position.y < -100 || position.y > 1080 + 100) {
                    _world.destroyEntity(*entity);
                    entities.erase(entities.begin() + i);
                }
            }
        }
    };

} // namespace rtype

#endif // R_TYPE_BULLETSYSTEM_HPP
