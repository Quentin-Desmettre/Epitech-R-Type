//
// Created by edo on 12/12/23.
//

#include "EntityFactory.hpp"
#include "PlayerComponent.hpp"
#include "SpriteComponent.hpp"
#include "PositionComponent.hpp"
#include "VelocityComponent.hpp"

aecs::World *EntityFactory::_world = nullptr;

void EntityFactory::setWorld(aecs::World *world)
{
    _world = world;
}

aecs::Entity &EntityFactory::toPlayer(aecs::Entity &entity)
{
    entity.addComponent<PlayerComponent>();
    entity.addComponent<SpriteComponent>("assets/sprites/pong.png", sf::Vector2f{30, 160}, sf::IntRect(0, 0, 45, 230));

    entity.addComponent<PositionComponent>(15, 0);
    entity.addComponent<VelocityComponent>(0, 0);
//    if (!_world->getIsServer()) {
    // If client
//    }
    return entity;
}

aecs::Entity &EntityFactory::createBackground()
{
    auto &back = _world->createEntity();
    back.addComponent<SpriteComponent>("assets/sprites/background.png", sf::Vector2f{1088, 640}, sf::IntRect{0, 0, 1920, 1080}, -10);
    back.addComponent<PositionComponent>(0, 0);
    return back;
}
