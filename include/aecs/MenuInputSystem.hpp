//
// Created by edo on 06/12/23.
//

#ifndef R_TYPE_MENUINPUTSYSTEM_HPP
#define R_TYPE_MENUINPUTSYSTEM_HPP

#include "InputComponent.hpp"
#include "aecs/SystemBase.hpp"
#include "aecs/World.hpp"
#include "rtype/EntityFactory.hpp"
#include <iostream>

namespace aecs
{
    class MenuInputSystem : public aecs::ALogicSystem
    {
      public:
        MenuInputSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities);
        ~MenuInputSystem() override = default;

        aecs::EntityChanges update(aecs::UpdateParams &updateParams) override;
    };
}

#endif // R_TYPE_MENUINPUTSYSTEM_HPP
