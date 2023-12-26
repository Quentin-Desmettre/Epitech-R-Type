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

        aecs::EntityChanges update(aecs::UpdateParams &updateParams) override
        {
            aecs::EntityChanges changes;
            for (auto &[_id, entity] : _entitiesMap) {
                auto &position = entity->getComponent<PositionComponent>();
                auto &velocity = entity->getComponent<VelocityComponent>();
                auto &sprite = entity->getComponent<SpriteComponent>();

                position.x += velocity.x * updateParams.deltaTime;
                position.y += velocity.y * updateParams.deltaTime;
                if (velocity.x != 0 || velocity.y != 0)
                    changes.editedEntities.push_back(entity->getId());
            }
            return changes;
        }
    };

} // namespace rtype

#endif // R_TYPE_PHYSICSSYSTEM_HPP
