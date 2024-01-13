//
// Created by qdesmettre on 05/12/23.
//

#ifndef R_TYPE_PHYSICSSYSTEM_HPP
#define R_TYPE_PHYSICSSYSTEM_HPP

#include "SystemBase.hpp"
#include "World.hpp"
#include "PositionComponent.hpp"
#include "SpriteComponent.hpp"
#include "VelocityComponent.hpp"
#include <iostream>

class PhysicsSystem : public aecs::ALogicSystem {
    public:
        PhysicsSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities);

        ~PhysicsSystem() override = default;

        aecs::EntityChanges update(aecs::UpdateParams &updateParams) override;
};

#endif // R_TYPE_PHYSICSSYSTEM_HPP
