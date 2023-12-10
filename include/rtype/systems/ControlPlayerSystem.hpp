//
// Created by edo on 06/12/23.
//

#ifndef R_TYPE_CONTROLPLAYERSYSTEM_HPP
#define R_TYPE_CONTROLPLAYERSYSTEM_HPP

#include "aecs/SystemBase.hpp"
#include "aecs/World.hpp"
#include "rtype/components/PositionComponent.hpp"
#include "rtype/components/VelocityComponent.hpp"
#include "rtype/components/SpriteComponent.hpp"
#include "rtype/components/MyPlayerComponent.hpp"
#include "rtype/components/BulletComponent.hpp"
#include <iostream>

namespace rtype
{

    class ControlPlayerSystem : public aecs::ALogicSystem
    {
      public:
        ControlPlayerSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
            ALogicSystem(world, entities, {typeid(VelocityComponent), typeid(MyPlayerComponent)})
        {
        }
        ~ControlPlayerSystem() override = default;

        void update(const std::vector<aecs::RenderInput> &inputs, float deltaTime) override
        {
            for (auto &[_id, entity] : _entitiesMap) {
                auto &velocity = entity->getComponent<VelocityComponent>();
                auto &my = entity->getComponent<MyPlayerComponent>();
                my.timeSinceLastShoot += deltaTime;

                velocity.x = 0;
                velocity.y = 0;
                bool space = false;
                bool shift = false;
                for (auto &input : inputs) {
                        if (input == sf::Keyboard::Key::Z) {
                                velocity.y += -50;
                        }
                        if (input == sf::Keyboard::Key::S) {
                                velocity.y += 50;
                        }
                        if (input == sf::Keyboard::Key::Q) {
                                velocity.x += -50;
                        }
                        if (input == sf::Keyboard::Key::D) {
                                velocity.x += 50;
                        }
                        if (input == sf::Keyboard::Key::Space) {
                            space = true;
                        }
                        if (input == sf::Keyboard::Key::LShift) {
                            shift = true;
                        }
                }
                if (space && !shift && my.timeSinceLastShoot > 6) {
                    auto &bullet = _world.createEntity();
                    bullet.addComponent<PositionComponent>(entity->getComponent<PositionComponent>().x + 48, entity->getComponent<PositionComponent>().y + 32);
                    bullet.addComponent<VelocityComponent>(100, 0);
                    bullet.addComponent<BulletComponent>();
                    bullet.addComponent<SpriteComponent>("assets/sprites/Bullet.png", sf::Vector2f(20 * 3, 14 * 3), sf::IntRect(0, 0, 20, 14));
                    my.timeSinceLastShoot = 0;
                }
                if (space && shift && my.timeSinceLastShoot > 6) {
                    auto &bullet = _world.createEntity();
                    bullet.addComponent<PositionComponent>(entity->getComponent<PositionComponent>().x + 48, entity->getComponent<PositionComponent>().y + 32);
                    bullet.addComponent<VelocityComponent>(100, 0);
                    bullet.addComponent<BulletComponent>();
                    bullet.addComponent<SpriteComponent>("assets/sprites/Bullet.png", sf::Vector2f(20 * 3, 14 * 3), sf::IntRect(0, 0, 20, 14));
                    my.timeSinceLastShoot = 0;
                }
            }
        }
    };

} // namespace rtype

#endif // R_TYPE_CONTROLPLAYERSYSTEM_HPP
