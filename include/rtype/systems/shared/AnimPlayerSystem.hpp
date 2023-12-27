//
// Created by edo on 06/12/23.
//

#ifndef R_TYPE_ANIMPLAYERSYSTEM_HPP
#define R_TYPE_ANIMPLAYERSYSTEM_HPP

#include "aecs/SystemBase.hpp"
#include "aecs/World.hpp"
#include "rtype/components/PlayerComponent.hpp"
#include "rtype/components/PositionComponent.hpp"
#include "rtype/components/SpriteComponent.hpp"
#include "rtype/components/VelocityComponent.hpp"
#include <iostream>

namespace rtype
{

    class AnimPlayerSystem : public aecs::ALogicSystem
    {
      public:
        AnimPlayerSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities);
        ~AnimPlayerSystem() override = default;

        aecs::EntityChanges update(aecs::UpdateParams &updateParams) override;
    };

} // namespace rtype

#endif // R_TYPE_ANIMPLAYERSYSTEM_HPP
