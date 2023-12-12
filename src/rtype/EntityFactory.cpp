//
// Created by edo on 12/12/23.
//

#include "rtype/EntityFactory.hpp"
#include "rtype/components/PositionComponent.hpp"
#include "rtype/components/SpriteComponent.hpp"
#include "rtype/components/VelocityComponent.hpp"
#include "rtype/components/MyPlayerComponent.hpp"
#include "rtype/components/ParallaxComponent.hpp"
#include "rtype/components/PlayerComponent.hpp"
#include "rtype/components/BulletComponent.hpp"

aecs::World *rtype::EntityFactory::_world = nullptr;

aecs::Entity &rtype::EntityFactory::createPlayer(bool main)
{
    auto &player = _world->createEntity();
    player.addComponent<PositionComponent>(0, 0);
    player.addComponent<VelocityComponent>(0, 0);
    player.addComponent<SpriteComponent>("assets/sprites/PlayerNew.png", sf::Vector2f{96, 96}, sf::IntRect(0, 0, 32, 32));
    if (main)
        player.addComponent<MyPlayerComponent>();
    player.addComponent<PlayerComponent>();
    return player;
}

aecs::Entity &rtype::EntityFactory::createBullet(sf::Vector2f position, sf::Vector2f velocity, bool big)
{
    auto &bullet = _world->createEntity();
    bullet.addComponent<PositionComponent>(position.x, position.y);
    if (!big)
        bullet.addComponent<VelocityComponent>(velocity.x * 1.5, velocity.y * 1.5);
    else
        bullet.addComponent<VelocityComponent>(velocity.x, velocity.y);
    bullet.addComponent<BulletComponent>();
    if (big)
        bullet.addComponent<SpriteComponent>("assets/sprites/BigBullet.png", sf::Vector2f(55 * 3, 29 * 3), sf::IntRect(0, 0, 55 , 29));
    else
        bullet.addComponent<SpriteComponent>("assets/sprites/Bullet.png", sf::Vector2f(20 * 3, 14 * 3), sf::IntRect(0, 0, 20, 14));
    return bullet;
}

aecs::Entity &rtype::EntityFactory::createEnemy(sf::Vector2f position, sf::Vector2f velocity)
{
    auto &enemy = _world->createEntity();
    enemy.addComponent<PositionComponent>(position.x, position.y);
    enemy.addComponent<VelocityComponent>(velocity.x, velocity.y);
    enemy.addComponent<SpriteComponent>("assets/sprites/Enemy.png", sf::Vector2f(96, 96), sf::IntRect(0, 0, 32, 32));
    return enemy;
}

aecs::Entity &rtype::EntityFactory::createBackground(int id, sf::Vector2f speed)
{
    auto &back = _world->createEntity();
    back.addComponent<SpriteComponent>("assets/sprites/Back" + std::to_string(id) + ".png", sf::Vector2f{1088, 640}, sf::IntRect {0, 0, 272 * 2 , 160});
    back.addComponent<PositionComponent>(0, 0);
    back.addComponent<ParallaxComponent>(speed);
    return back;
}

void rtype::EntityFactory::setWorld(aecs::World *world)
{
    _world = world;
}