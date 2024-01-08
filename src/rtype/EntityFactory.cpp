//
// Created by edo on 12/12/23.
//

#include "rtype/EntityFactory.hpp"
#include "aecs/InputComponent.hpp"
#include "rtype/components/AnimComponent.hpp"
#include "rtype/components/BulletComponent.hpp"
#include "rtype/components/DamageCollisionComponent.hpp"
#include "rtype/components/HPComponent.hpp"
#include "rtype/components/MonsterComponent.hpp"
#include "rtype/components/ParallaxComponent.hpp"
#include "rtype/components/PlayerComponent.hpp"
#include "rtype/components/PositionComponent.hpp"
#include "rtype/components/PowerComponent.hpp"
#include "rtype/components/ShaderComponent.hpp"
#include "rtype/components/SpriteComponent.hpp"
#include "rtype/components/TextComponent.hpp"
#include "rtype/components/VelocityComponent.hpp"
#include <memory>

aecs::World *rtype::EntityFactory::_world = nullptr;

aecs::Entity &rtype::EntityFactory::toPlayer(aecs::Entity &entity)
{
    auto &component = entity.getComponent<PlayerComponent>();
    entity.addComponent<SpriteComponent>("assets/sprites/PlayerNew.png", sf::Vector2f{96, 96},
                                         sf::IntRect(0, 0, 32, 32));
    entity.addComponent<PositionComponent>(0, 0);
    entity.addComponent<VelocityComponent>(0, 0);
    entity.addComponent<DamageCollisionComponent>(0, 0);
    entity.addComponent<HPComponent>(50);
    if (!_world->getIsServer()) {
        std::shared_ptr<sf::Shader> shader = std::make_shared<sf::Shader>();
        entity.addComponent<AnimComponent>(1);
        shader->loadFromFile("shaders/PlayerColor.frag", sf::Shader::Fragment);
        float colors[4] = {0, 0, 144, 216};
        shader->setUniform("hue", colors[component.playerId]);
        shader->setUniform("saturation", component.playerId == 0 ? 0.f : 1.f);
        entity.addComponent<ShaderComponent>(shader);
    }
    return entity;
}

aecs::Entity &rtype::EntityFactory::createPlayer()
{
    auto &player = _world->createEntity();
    player.addComponent<PlayerComponent>();
    toPlayer(player);

    return player;
}

aecs::Entity &rtype::EntityFactory::toBullet(aecs::Entity &entity)
{
    auto &component = entity.getComponent<BulletComponent>();
    bool big = component.big;
    entity.addComponent<HPComponent>(big ? 50 : 5);
    if (big)
        entity.addComponent<SpriteComponent>("assets/sprites/BigBullet.png", sf::Vector2f(55 * 3, 29 * 3),
                                             sf::IntRect(0, 0, 55, 29));
    else
        entity.addComponent<SpriteComponent>("assets/sprites/Bullet.png", sf::Vector2f(20 * 3, 14 * 3),
                                             sf::IntRect(0, 0, 20, 14));
    if (!_world->getIsServer()) {
        entity.addComponent<AnimComponent>(0.5);
        entity.addComponent<PositionComponent>();
        entity.addComponent<DamageCollisionComponent>();
        entity.addComponent<VelocityComponent>();
    }
    return entity;
}

aecs::Entity &rtype::EntityFactory::createBullet(sf::Vector2f position, sf::Vector2f velocity, int team, bool big)
{
    auto &bullet = _world->createEntity();
    bullet.addComponent<BulletComponent>(big);
    toBullet(bullet);

    bullet.addComponent<DamageCollisionComponent>(
        team, big ? 50 : 5, big ? DamageCollisionComponent::LG_BULLET : DamageCollisionComponent::SM_BULLET);
    bullet.addComponent<PositionComponent>(position.x, position.y);
    if (!big)
        bullet.addComponent<VelocityComponent>(velocity.x * 1.5, velocity.y * 1.5);
    else
        bullet.addComponent<VelocityComponent>(velocity.x, velocity.y);
    return bullet;
}

aecs::Entity &rtype::EntityFactory::toEnemy(aecs::Entity &entity)
{
    auto &component = entity.getComponent<MonsterComponent>();
    bool lil = component._lil;
    if (lil)
        entity.addComponent<SpriteComponent>("assets/sprites/LilMonster.png", sf::Vector2f(63, 48),
                                             sf::IntRect(21 * (rand() % 5), 0, 21, 16));
    else
        entity.addComponent<SpriteComponent>("assets/sprites/Monster.png", sf::Vector2f(156, 102),
                                             sf::IntRect(52 * (rand() % 5), 0, 52, 34));
    if (!_world->getIsServer()) {
        entity.addComponent<AnimComponent>(1);
        entity.addComponent<PositionComponent>();
        entity.addComponent<DamageCollisionComponent>();
        entity.addComponent<VelocityComponent>();
        entity.addComponent<HPComponent>();
    }
    return entity;
}

aecs::Entity &rtype::EntityFactory::createEnemy(sf::Vector2f position, sf::Vector2f velocity, bool lil)
{
    auto &enemy = _world->createEntity();
    position.x = 1088 + rand() % 200;
    enemy.addComponent<MonsterComponent>(lil);
    toEnemy(enemy);
    enemy.addComponent<PositionComponent>(position.x, position.y);
    if (lil)
        enemy.addComponent<VelocityComponent>(velocity.x * 1.25, velocity.y * 1.25);
    else
        enemy.addComponent<VelocityComponent>(velocity.x * 0.75, velocity.y * 0.75);
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
                                       sf::IntRect{0, 0, 272 * 2, 160}, -6 + id);
    back.addComponent<PositionComponent>(0, 0);
    back.addComponent<ParallaxComponent>(speed);
    return back;
}

aecs::Entity &rtype::EntityFactory::createPower(sf::Vector2f position, bool isPowerUp)
{
    auto &power = _world->createEntity();
    power.addComponent<PowerComponent>(isPowerUp);
    toPower(power, isPowerUp);
    power.addComponent<PositionComponent>(position.x, position.y);
    power.addComponent<VelocityComponent>(-20, 4);
    return power;
}

aecs::Entity &rtype::EntityFactory::toPower(aecs::Entity &entity, bool isPowerUp)
{
    std::string path = isPowerUp ? "assets/sprites/PowerUp.png" : "assets/sprites/PowerDown.png";

    entity.addComponent<SpriteComponent>(path, sf::Vector2f{57, 42}, sf::IntRect{0, 0, 19, 14});
    if (!_world->getIsServer()) {
        entity.addComponent<PositionComponent>();
        entity.addComponent<VelocityComponent>();
    }
    return entity;
}

aecs::Entity &rtype::EntityFactory::createInputs(int input, std::function<void()> &&onInput)
{
    auto &inputs = _world->createEntity();
    inputs.addComponent<aecs::InputComponent>(input, std::move(onInput));
    return inputs;
}

aecs::Entity &rtype::EntityFactory::createButton(const std::string &text, int fontSize, sf::Color color, int zIndex, sf::Vector2f pos, std::function<void()> &&onClick)
{
    auto &buttons = _world->createEntity();

    buttons.addComponent<TextComponent>(text, fontSize, color, zIndex, std::move(onClick));
    buttons.addComponent<PositionComponent>(pos.x, pos.y);
    return buttons;
}

void rtype::EntityFactory::setWorld(aecs::World *world)
{
    _world = world;
}
