//
// Created by edo on 06/12/23.
//

#ifndef R_TYPE_CONTROLPLAYERSYSTEM_HPP
#define R_TYPE_CONTROLPLAYERSYSTEM_HPP

#include "SystemBase.hpp"
#include "World.hpp"
#include "EntityFactory.hpp"
#include "PlayerComponent.hpp"
#include "PositionComponent.hpp"
#include "SpriteComponent.hpp"
#include "VelocityComponent.hpp"
#include <iostream>

class ControlPlayerSystem : public aecs::ALogicSystem {
    public:
        ControlPlayerSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities);

        ~ControlPlayerSystem() override = default;

        aecs::EntityChanges update(aecs::UpdateParams &updateParams) override;
};

#endif // R_TYPE_CONTROLPLAYERSYSTEM_HPP
