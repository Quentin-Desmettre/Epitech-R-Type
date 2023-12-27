//
// Created by edo on 08/12/23.
//

#ifndef R_TYPE_PARALLAXSYSTEM_HPP
#define R_TYPE_PARALLAXSYSTEM_HPP

#include "aecs/SystemBase.hpp"
#include "aecs/World.hpp"
#include "rtype/components/ParallaxComponent.hpp"
#include "rtype/components/PositionComponent.hpp"
#include "rtype/components/SpriteComponent.hpp"
#include "rtype/components/VelocityComponent.hpp"
namespace rtype
{

    class ParallaxSystem : public aecs::ALogicSystem
    {
      public:
        ParallaxSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities);
        ~ParallaxSystem() override = default;

        aecs::EntityChanges update(aecs::UpdateParams &updateParams) override;
    };

} // namespace rtype

#endif // R_TYPE_PARALLAXSYSTEM_HPP
