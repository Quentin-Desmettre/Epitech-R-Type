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

        static sf::Rect<float> getRect(const std::shared_ptr<aecs::Entity> &entity);

        static bool killed(const std::shared_ptr<aecs::Entity> &entity, const std::shared_ptr<aecs::Entity> &entity2);

        aecs::EntityChanges update(aecs::UpdateParams &updateParams) override;
    };

} // namespace rtype

#endif // R_TYPE_DAMAGECOLLISIONSYSTEM_HPP
