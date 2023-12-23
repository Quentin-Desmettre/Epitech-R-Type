//
// Created by edo on 12/12/23.
//

#include "rtype/EntityFactory.hpp"
#include "rtype/components/AnimComponent.hpp"
#include "rtype/components/BulletComponent.hpp"
#include "rtype/components/DamageCollisionComponent.hpp"
#include "rtype/components/HPComponent.hpp"
#include "rtype/components/MonsterComponent.hpp"
#include "rtype/components/MyPlayerComponent.hpp"
#include "rtype/components/ParallaxComponent.hpp"
#include "rtype/components/PlayerComponent.hpp"
#include "rtype/components/PositionComponent.hpp"
#include "rtype/components/ShaderComponent.hpp"
#include "rtype/components/SpriteComponent.hpp"
#include "rtype/components/VelocityComponent.hpp"
#include <memory>
#include "rtype/components/SpriteComponent.hpp"
#include "rtype/components/VelocityComponent.hpp"

aecs::World *rtype::EntityFactory::_world = nullptr;

aecs::Entity &rtype::EntityFactory::createPlayer(bool main, int nb, size_t id)
{
    auto &player = _world->createEntity(id);
    player.addComponent<PositionComponent>(0, 0);
    player.addComponent<AnimComponent>(1);
    player.addComponent<VelocityComponent>(0, 0);
    player.addComponent<SpriteComponent>("assets/sprites/PlayerNew.png", sf::Vector2f{96, 96},
                                         sf::IntRect(0, 0, 32, 32));
    if (main)
        player.addComponent<MyPlayerComponent>();
    player.addComponent<PlayerComponent>();
    player.addComponent<DamageCollisionComponent>(0, 0);
    player.addComponent<HPComponent>(50);
    std::shared_ptr<sf::Shader> shader = std::make_shared<sf::Shader>();
    shader->loadFromFile("src/client/Shaders/PlayerColor.frag", sf::Shader::Fragment);
    shader->setUniform("hue", 50 * nb - 50);
    shader->setUniform("saturation", float(!main));
    player.addComponent<ShaderComponent>(shader);

    return player;
}

aecs::Entity &rtype::EntityFactory::createBullet(sf::Vector2f position, sf::Vector2f velocity, int team, bool big, size_t id)
{
    auto &bullet = _world->createEntity(id);
    bullet.addComponent<AnimComponent>(0.5);
    bullet.addComponent<DamageCollisionComponent>(
        team, big ? 50 : 5, big ? DamageCollisionComponent::LG_BULLET : DamageCollisionComponent::SM_BULLET);
    bullet.addComponent<HPComponent>(big ? 50 : 5);
    bullet.addComponent<PositionComponent>(position.x, position.y);
    if (!big)
        bullet.addComponent<VelocityComponent>(velocity.x * 1.5, velocity.y * 1.5);
    else
        bullet.addComponent<VelocityComponent>(velocity.x, velocity.y);
    bullet.addComponent<BulletComponent>();
    if (big)
        bullet.addComponent<SpriteComponent>("assets/sprites/BigBullet.png", sf::Vector2f(55 * 3, 29 * 3),
                                             sf::IntRect(0, 0, 55, 29));
    else
        bullet.addComponent<SpriteComponent>("assets/sprites/Bullet.png", sf::Vector2f(20 * 3, 14 * 3),
                                             sf::IntRect(0, 0, 20, 14));
    return bullet;
}

aecs::Entity &rtype::EntityFactory::createEnemy(sf::Vector2f position, sf::Vector2f velocity, bool lil, size_t id)
{
    auto &enemy = _world->createEntity(id);
    position.x = 1088 + rand() % 200;
    enemy.addComponent<PositionComponent>(position.x, position.y);
    if (lil)
        enemy.addComponent<VelocityComponent>(velocity.x * 1.5, velocity.y * 1.5);
    else
        enemy.addComponent<VelocityComponent>(velocity.x, velocity.y);
    if (lil)
        enemy.addComponent<SpriteComponent>("assets/sprites/LilMonster.png", sf::Vector2f(63, 48),
                                            sf::IntRect(21 * (rand() % 5), 0, 21, 16));
    else
        enemy.addComponent<SpriteComponent>("assets/sprites/Monster.png", sf::Vector2f(156, 102),
                                            sf::IntRect(52 * (rand() % 5), 0, 52, 34));
    enemy.addComponent<AnimComponent>(1);
    enemy.addComponent<MonsterComponent>(lil);
    if (lil) {
        enemy.addComponent<DamageCollisionComponent>(1, 5);
        enemy.addComponent<HPComponent>(1);
    } else {
        enemy.addComponent<DamageCollisionComponent>(1, 10);
        enemy.addComponent<HPComponent>(10);
    }
    return enemy;
}

aecs::Entity &rtype::EntityFactory::createBackground(int id, sf::Vector2f speed)
{
    auto &back = _world->createEntity();
    back.addComponent<SpriteComponent>("assets/sprites/Back" + std::to_string(id) + ".png", sf::Vector2f{1088, 640},
                                       sf::IntRect{0, 0, 272 * 2, 160});
    back.addComponent<PositionComponent>(0, 0);
    back.addComponent<ParallaxComponent>(speed);
    return back;
}

void rtype::EntityFactory::setWorld(aecs::World *world)
{
    _world = world;
}