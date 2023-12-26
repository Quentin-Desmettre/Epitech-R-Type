//
// Created by edo on 06/12/23.
//

#ifndef R_TYPE_CONTROLPLAYERSYSTEM_HPP
#define R_TYPE_CONTROLPLAYERSYSTEM_HPP

#include "aecs/SystemBase.hpp"
#include "aecs/World.hpp"
#include "rtype/EntityFactory.hpp"
#include "rtype/components/BulletComponent.hpp"
#include "rtype/components/MyPlayerComponent.hpp"
#include "rtype/components/NetworkTagComponent.hpp"
#include "rtype/components/PlayerComponent.hpp"
#include "rtype/components/PositionComponent.hpp"
#include "rtype/components/SpriteComponent.hpp"
#include "rtype/components/VelocityComponent.hpp"
#include <iostream>
namespace rtype
{
    class ControlPlayerSystem : public aecs::ALogicSystem
    {
      public:
        ControlPlayerSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
            ALogicSystem(world, entities,
                         {typeid(VelocityComponent), typeid(SpriteComponent), typeid(PlayerComponent),
                          typeid(PositionComponent)})
        {
        }
        ~ControlPlayerSystem() override = default;

        aecs::EntityChanges update(aecs::UpdateParams &updateParams) override
        {
            aecs::EntityChanges changes;
            for (auto &[_id, entity] : _entitiesMap) {
                const auto &position = entity->getComponent<PositionComponent>();
                auto &velocity = entity->getComponent<VelocityComponent>();
                auto &sprite = entity->getComponent<SpriteComponent>();
                auto &my = entity->getComponent<PlayerComponent>();
                my.timeSinceLastShoot += updateParams.deltaTime;
                changes.editedEntities.push_back(entity->getId());

                velocity.x = 0;
                velocity.y = 0;
                bool space = false;
                bool shift = false;

                auto &inputs = _world.getInputs();
                for (auto &[playerId, clientInputs] : inputs) {
                    if (playerId != my.playerId) { // Apply inputs only for the player we're controlling
                        continue;
                    }
                    for (auto &input : clientInputs) {
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
                }
                if (space && !shift && my.timeSinceLastShoot > 3) {
                    EntityFactory::createBullet(sf::Vector2f(position.x + 48, position.y + 32), sf::Vector2f(100, 0),
                                                0);
                    my.timeSinceLastShoot = 0;
                }
                if (shift && my.timeSinceLastShoot > 6) {
                    my.timeInShift += updateParams.deltaTime;
                }
                if (!shift) {
                    my.timeInShift = 0;
                }
                int color = 255 - (int)(float(std::min(6, int(my.timeInShift)) / 6.0 * 255));
                // faire clignotter le sprite
                if (my.timeInShift < 6 || int(my.timeInShift) % 2 == 0)
                    sprite.sprite.setColor(sf::Color(255, color * 2 / 3 + 85, color * 2 / 3 + 85, 255));
                else
                    sprite.sprite.setColor(sf::Color(255, 255, 255, 255));

                if (space && shift && my.timeInShift > 6) {
                    changes.editedEntities.push_back(
                        EntityFactory::createBullet(sf::Vector2f(position.x + 48, position.y + 2), sf::Vector2f(100, 0),
                                                    0, true)
                            .getId());
                    my.timeSinceLastShoot = 0;
                    my.timeInShift = 0;
                }
            }
            return changes;
        }
    };

} // namespace rtype

#endif // R_TYPE_CONTROLPLAYERSYSTEM_HPP
