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
        BulletSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities);
        ~BulletSystem() override = default;

        aecs::EntityChanges update(aecs::UpdateParams &updateParams) override;
    };

} // namespace rtype

#endif // R_TYPE_BULLETSYSTEM_HPP
