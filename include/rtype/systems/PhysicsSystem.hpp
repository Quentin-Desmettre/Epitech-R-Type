//
// Created by qdesmettre on 05/12/23.
//

#ifndef R_TYPE_PHYSICSSYSTEM_HPP
#define R_TYPE_PHYSICSSYSTEM_HPP

#include "aecs/SystemBase.hpp"
#include "aecs/World.hpp"
#include "rtype/components/PositionComponent.hpp"
#include "rtype/components/VelocityComponent.hpp"
#include "rtype/components/SpriteComponent.hpp"
#include <iostream>

namespace rtype
{

    class PhysicsSystem : public aecs::ALogicSystem
    {
      public:
        PhysicsSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
            ALogicSystem(world, entities, {typeid(PositionComponent), typeid(VelocityComponent), typeid(SpriteComponent)})
        {
        }
        ~PhysicsSystem() override = default;

        void update(const std::vector<aecs::RenderInput> &inputs, float deltaTime) override
        {
            for (auto &[_id, entity] : _entitiesMap) {
                auto &position = entity->getComponent<PositionComponent>();
                auto &velocity = entity->getComponent<VelocityComponent>();
                auto &sprite = entity->getComponent<SpriteComponent>();


                position.x += velocity.x * deltaTime;
                position.y += velocity.y * deltaTime;
                if (position.x < 0 && velocity.x < 0) {
                    position.x = 0;
                    velocity.x = 0;
                }
                if (position.y < 0 && velocity.y < 0) {
                    position.y = 0;
                    velocity.y = 0;
                }
                if (position.x > 1440 - sprite._size.x && velocity.x > 0) {
                    position.x = 1440 - sprite._size.x;
                    velocity.x = 0;
                }
                if (position.y > 810 - sprite._size.y && velocity.y > 0) {
                    position.y = 810 - sprite._size.y;
                    velocity.y = 0;
                }
            }
        }
    };

} // namespace rtype

#endif // R_TYPE_PHYSICSSYSTEM_HPP
