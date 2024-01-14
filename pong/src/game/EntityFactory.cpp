//
// Created by edo on 12/12/23.
//

#include "EntityFactory.hpp"
#include "PlayerComponent.hpp"
#include "SpriteComponent.hpp"
#include "PositionComponent.hpp"
#include "VelocityComponent.hpp"
#include "InputComponent.hpp"
#include "TextComponent.hpp"
#include "BallComponent.hpp"
#include "TagComponent.hpp"

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

aecs::Entity &EntityFactory::createInputs(int input, std::function<void()> &&onInput)
{
    auto &inputs = _world->createEntity();
    inputs.addComponent<aecs::InputComponent>(input, std::move(onInput));
    return inputs;
}

aecs::Entity &EntityFactory::createButton(const std::string &text, int fontSize, sf::Color color, int zIndex, sf::Vector2f pos, std::function<void()> &&onClick)
{
    auto &buttons = _world->createEntity();

    buttons.addComponent<TextComponent>(text, fontSize, color, true, zIndex, std::move(onClick));
    buttons.addComponent<PositionComponent>(pos.x, pos.y);
    return buttons;
}

aecs::Entity &EntityFactory::createBall()
{
    auto &ball = _world->createEntity();
    ball.addComponent<BallComponent>();
    ball.addComponent<SpriteComponent>("assets/sprites/ball.png", sf::Vector2f{30, 30}, sf::IntRect(0, 0, 105, 105));
    ball.addComponent<PositionComponent>(544, 320);
    ball.addComponent<VelocityComponent>(0, 0);
    return ball;
}

aecs::Entity &EntityFactory::toBall(aecs::Entity &entity)
{
    entity.addComponent<SpriteComponent>("assets/sprites/ball.png", sf::Vector2f{30, 30}, sf::IntRect(0, 0, 105, 105));
    entity.addComponent<PositionComponent>(544, 320);
    entity.addComponent<VelocityComponent>(0, 0);
    return entity;
}

aecs::Entity &EntityFactory::createPlayer(bool isAI)
{
    auto &player = _world->createEntity();
    player.addComponent<PlayerComponent>(isAI);
    player.addComponent<SpriteComponent>("assets/sprites/pong.png", sf::Vector2f{30, 160}, sf::IntRect(0, 0, 45, 230));

    if (isAI)
        player.addComponent<PositionComponent>(1043, 0);
    else
        player.addComponent<PositionComponent>(15, 0);
    player.addComponent<VelocityComponent>(0, 0);
    return player;
}

aecs::Entity &EntityFactory::createScore(bool isAI)
{
    auto &score = _world->createEntity();
    score.addComponent<TextComponent>("Score: 0", 20, sf::Color::White, true, 100);
    if (isAI) {
        score.addComponent<PositionComponent>(1088 / 2 + 150, 10);
        score.addComponent<TagComponent>("scoreAI");
    } else {
        score.addComponent<PositionComponent>(1088 / 2 - 150, 10);
        score.addComponent<TagComponent>("scorePlayer");
    }
    return score;
}