//
// Created by Clément Vandeville on 14/01/2024.
//

#ifndef PONG_BALLSYSTEM_HPP
#define PONG_BALLSYSTEM_HPP

#include "SystemBase.hpp"
#include "World.hpp"
#include "EntityFactory.hpp"
#include "PlayerComponent.hpp"
#include "PositionComponent.hpp"
#include "SpriteComponent.hpp"
#include "VelocityComponent.hpp"
#include <iostream>

class BallSystem : public aecs::ALogicSystem {
    public:
        BallSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities);

        ~BallSystem() override = default;

        sf::Rect<float> getRect(const std::shared_ptr<aecs::Entity> &entity);
        sf::Vector2f moveTo(sf::Vector2f from, sf::Vector2f to, float speed);

        aecs::EntityChanges update(aecs::UpdateParams &updateParams) override;
};

#endif //PONG_BALLSYSTEM_HPP
