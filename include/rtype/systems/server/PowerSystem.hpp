//
// Created by edo on 12/12/23.
//

#ifndef R_TYPE_POWERSYSTEM_HPP
#define R_TYPE_POWERSYSTEM_HPP

#include "SFML/Graphics.hpp"
#include "aecs/SystemBase.hpp"
#include "aecs/World.hpp"
#include "rtype/components/DamageCollisionComponent.hpp"
#include "rtype/components/PlayerComponent.hpp"
#include "rtype/components/PositionComponent.hpp"
#include "rtype/components/PowerComponent.hpp"
#include "rtype/components/SpriteComponent.hpp"
#include <iostream>
namespace rtype
{
    class PowerSystem : public aecs::ALogicSystem
    {
      public:

        PowerSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities);
        ~PowerSystem() override = default;

        void onEntityAdded(const aecs::EntityPtr &entity) override;
        void onEntityRemoved(const aecs::EntityPtr &entity) override;
        void onEntityModified(const aecs::EntityPtr &entity) override;

        aecs::EntityChanges update(aecs::UpdateParams &updateParams) override;

      private:
        static sf::Rect<float> getRect(const std::shared_ptr<aecs::Entity> &entity);
        aecs::EntityPtr &getPlayerId();
    };

} // namespace rtype

#endif // R_TYPE_DAMAGECOLLISIONSYSTEM_HPP
