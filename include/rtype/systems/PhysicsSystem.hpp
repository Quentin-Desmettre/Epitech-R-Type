//
// Created by qdesmettre on 05/12/23.
//

#ifndef R_TYPE_PHYSICSSYSTEM_HPP
#define R_TYPE_PHYSICSSYSTEM_HPP

#include "aecs/SystemBase.hpp"
#include "aecs/World.hpp"
#include "rtype/components/PositionComponent.hpp"
#include "rtype/components/SpriteComponent.hpp"
#include "rtype/components/VelocityComponent.hpp"
#include <iostream>

namespace rtype
{

    class PhysicsSystem : public aecs::ALogicSystem
    {
      public:
        PhysicsSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
            ALogicSystem(world, entities,
                         {typeid(PositionComponent), typeid(VelocityComponent), typeid(SpriteComponent)})
        {
        }
        ~PhysicsSystem() override = default;

        void update(const aecs::UpdateParams &updateParams) override
        {
            for (auto &[_id, entity] : _entitiesMap) {
                auto &position = entity->getComponent<PositionComponent>();
                auto &velocity = entity->getComponent<VelocityComponent>();
                auto &sprite = entity->getComponent<SpriteComponent>();
                std::vector<std::byte> encoded = position.encode();
                position.decode(encoded);

                position.x += velocity.x * updateParams.deltaTime;
                position.y += velocity.y * updateParams.deltaTime;
            }
        }
    };

} // namespace rtype

#endif // R_TYPE_PHYSICSSYSTEM_HPP
