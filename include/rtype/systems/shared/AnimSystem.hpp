//
// Created by edo on 12/12/23.
//

#ifndef R_TYPE_ANIMSYSTEM_HPP
#define R_TYPE_ANIMSYSTEM_HPP

#include "aecs/SystemBase.hpp"
#include "aecs/World.hpp"
#include "rtype/components/AnimComponent.hpp"
#include "rtype/components/SpriteComponent.hpp"
#include <iostream>
namespace rtype
{
    class AnimSystem : public aecs::ALogicSystem
    {
      public:
        AnimSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities);
        ~AnimSystem() override = default;

        aecs::EntityChanges update(aecs::UpdateParams &updateParams) override;
    };

} // namespace rtype

#endif // R_TYPE_ANIMSYSTEM_HPP
