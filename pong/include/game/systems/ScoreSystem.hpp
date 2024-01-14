//
// Created by Clément Vandeville on 14/01/2024.
//

#ifndef PONG_SCORESYSTEM_HPP
#define PONG_SCORESYSTEM_HPP


#include "SystemBase.hpp"
#include "World.hpp"
#include "PositionComponent.hpp"
#include "SpriteComponent.hpp"
#include "VelocityComponent.hpp"
#include <iostream>

class ScoreSystem : public aecs::ALogicSystem {
    public:
        ScoreSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities);

        ~ScoreSystem() override = default;

        aecs::EntityChanges update(aecs::UpdateParams &updateParams) override;
};



#endif //PONG_SCORESYSTEM_HPP
