//
// Created by edo on 12/12/23.
//

#ifndef R_TYPE_DAMAGECOLLISIONSYSTEM_HPP
#define R_TYPE_DAMAGECOLLISIONSYSTEM_HPP

#include "SFML/Graphics.hpp"
#include "aecs/SystemBase.hpp"
#include "aecs/World.hpp"
#include "rtype/components/DamageCollisionComponent.hpp"
#include "rtype/components/HPComponent.hpp"
#include "rtype/components/MonsterComponent.hpp"
#include "rtype/components/PositionComponent.hpp"
#include "rtype/components/SpriteComponent.hpp"
#include <iostream>
namespace rtype
{
    class DamageCollisionSystem : public aecs::ALogicSystem
    {
      public:
        DamageCollisionSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities);
        ~DamageCollisionSystem() override = default;

        aecs::EntityChanges update(aecs::UpdateParams &updateParams) override;

        void onEntityAdded(const aecs::EntityPtr &entity) override;
        void onEntityRemoved(const aecs::EntityPtr &entity) override;
        void onEntityModified(const aecs::EntityPtr &entity) override;

      private:
        static sf::Rect<float> getRect(aecs::Entity *entity, const PositionComponent &position);
        bool killed(const std::shared_ptr<aecs::Entity> &entity, const std::shared_ptr<aecs::Entity> &entity2);
        void addPowerUp(aecs::EntityChanges &changes);
        float notRootedDistance(const PositionComponent &position, const PositionComponent &position2);

        std::vector<std::shared_ptr<aecs::Entity>> entities;
    };

} // namespace rtype

#endif // R_TYPE_DAMAGECOLLISIONSYSTEM_HPP
