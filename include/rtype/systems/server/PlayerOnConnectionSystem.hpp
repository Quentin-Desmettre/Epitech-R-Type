//
// Created by edo on 26/12/23.
//

#ifndef R_TYPE_PLAYERONCONNECTIONSYSTEM_HPP
#define R_TYPE_PLAYERONCONNECTIONSYSTEM_HPP

#include "aecs/SystemBase.hpp"
#include "aecs/World.hpp"
#include "rtype/components/ParallaxComponent.hpp"
#include "rtype/components/PositionComponent.hpp"
#include "rtype/components/SpriteComponent.hpp"
#include "rtype/components/VelocityComponent.hpp"
#include <iostream>

namespace rtype
{

    class PlayerOnConnectionSystem : public aecs::ALogicSystem
    {
      public:
        PlayerOnConnectionSystem(aecs::World &world,
                                 const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities);
        ~PlayerOnConnectionSystem() override = default;

        aecs::EntityChanges update(aecs::UpdateParams &updateParams) override;

        void onEntityAdded(const aecs::EntityPtr &entity) override;

        void onEntityModified(const aecs::EntityPtr &entity) override;
    };

} // namespace rtype

#endif // R_TYPE_PLAYERONCONNECTIONSYSTEM_HPP
