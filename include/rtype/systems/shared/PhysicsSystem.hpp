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
        PhysicsSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities);
        ~PhysicsSystem() override = default;

        void onEntityAdded(const aecs::EntityPtr &entity) override;
        void onEntityRemoved(const aecs::EntityPtr &entity) override;
        void onEntityModified(const aecs::EntityPtr &entity) override;

        aecs::EntityChanges update(aecs::UpdateParams &updateParams) override;

      private:
        enum CollisionDirection {
            NONE = 0,
            TOP = 1,
            BOTTOM = 2,
            LEFT = 4,
            RIGHT = 8,
        };
        std::vector<std::pair<aecs::EntityPtr, PhysicsSystem::CollisionDirection>> getCollisions(const aecs::EntityPtr &entity);
        static CollisionDirection getCollisionDirection(const aecs::EntityPtr &entity1, const aecs::EntityPtr &entity2);
        static bool isOutOfScreen(const PositionComponent &position, const sf::Vector2f &size);

        std::map<std::size_t, aecs::EntityPtr> _movableEntities;
        std::map<std::size_t, aecs::EntityPtr> _collidableEntities;
    };

} // namespace rtype

#endif // R_TYPE_PHYSICSSYSTEM_HPP
