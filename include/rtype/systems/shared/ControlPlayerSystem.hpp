//
// Created by edo on 06/12/23.
//

#ifndef R_TYPE_CONTROLPLAYERSYSTEM_HPP
#define R_TYPE_CONTROLPLAYERSYSTEM_HPP

#include "aecs/SystemBase.hpp"
#include "aecs/World.hpp"
#include "rtype/EntityFactory.hpp"
#include "rtype/components/BulletComponent.hpp"
#include "rtype/components/PlayerComponent.hpp"
#include "rtype/components/PositionComponent.hpp"
#include "rtype/components/SpriteComponent.hpp"
#include "rtype/components/VelocityComponent.hpp"
#include <iostream>

namespace rtype
{
    class ControlPlayerSystem : public aecs::ALogicSystem
    {
      public:
        ControlPlayerSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities);
        ~ControlPlayerSystem() override = default;

        aecs::EntityChanges update(aecs::UpdateParams &updateParams) override;

      private:
        sf::Keyboard::Key reverseKey(sf::Keyboard::Key key, bool reverse);
    };

} // namespace rtype

#endif // R_TYPE_CONTROLPLAYERSYSTEM_HPP
