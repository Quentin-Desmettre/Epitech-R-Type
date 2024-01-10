//
// Created by edo on 06/12/23.
//

#include "rtype/systems/shared/ControlPlayerSystem.hpp"

namespace rtype
{
    ControlPlayerSystem::ControlPlayerSystem(aecs::World &world,
                                             const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
        ALogicSystem(
            world, entities,
            {typeid(VelocityComponent), typeid(SpriteComponent), typeid(PlayerComponent), typeid(PositionComponent)})
    {
    }

    aecs::EntityChanges ControlPlayerSystem::update(aecs::UpdateParams &updateParams)
    {
        aecs::EntityChanges changes;
        for (auto &[_id, entity] : _entitiesMap) {
            const auto &position = entity->getComponent<PositionComponent>();
            auto &velocity = entity->getComponent<VelocityComponent>();
            auto &sprite = entity->getComponent<SpriteComponent>();
            auto &my = entity->getComponent<PlayerComponent>();
            my.timeSinceLastShoot += updateParams.deltaTime;
            my.timeLeftShootPowerUp -= updateParams.deltaTime * 0.1;
            my.timeLeftMovePowerDown -= updateParams.deltaTime * 0.1;
            changes.editedEntities.insert(entity->getId());

            velocity.x = 0;
            velocity.y = 0;
            bool space = false;
            bool shift = false;

            for (auto &[playerId, clientInputs] : updateParams.inputs) {
                if (playerId != my.playerId) // Apply inputs only for the player we're controlling
                    continue;
                bool reverse = my.timeLeftMovePowerDown > 0;
                for (auto &input : clientInputs) {
                    if (input == reverseKey(sf::Keyboard::Key::Z, reverse))
                        velocity.y += -50;
                    if (input == reverseKey(sf::Keyboard::Key::S, reverse))
                        velocity.y += 50;
                    if (input == reverseKey(sf::Keyboard::Key::Q, reverse))
                        velocity.x += -50;
                    if (input == reverseKey(sf::Keyboard::Key::D, reverse))
                        velocity.x += 50;
                    if (input == sf::Keyboard::Key::Space)
                        space = true;
                    if (input == sf::Keyboard::Key::LShift)
                        shift = true;
                }
            }
            if (space && !shift && my.timeSinceLastShoot > 3) {
                if (_world.getIsServer()) {
                    EntityFactory::createBullet(sf::Vector2f(position.x + 48, position.y), sf::Vector2f(100, 0),
                                                0);
                    if (my.timeLeftShootPowerUp > 0)
                        EntityFactory::createBullet(sf::Vector2f(position.x + 48, position.y + 32),
                                                    sf::Vector2f(100, 0), 0);
                }
                my.timeSinceLastShoot = 0;
            }
            if (shift && my.timeSinceLastShoot > 6)
                my.timeInShift += updateParams.deltaTime;
            if (!shift)
                my.timeInShift = 0;
            int color = 255 - (int)(float(std::min(6, int(my.timeInShift)) / 6.0 * 255));
            // faire clignotter le sprite
            if (my.timeInShift < 6 || int(my.timeInShift) % 2 == 0)
                sprite.sprite.setColor(sf::Color(255, color * 2 / 3 + 85, color * 2 / 3 + 85, 255));
            else
                sprite.sprite.setColor(sf::Color(255, 255, 255, 255));

            if (space && shift && my.timeInShift > 6) {
                if (_world.getIsServer())
                    changes.editedEntities.insert(
                        EntityFactory::createBullet(sf::Vector2f(position.x + 48, position.y), sf::Vector2f(100, 0),
                                                    0, true)
                            .getId());
                my.timeSinceLastShoot = 0;
                my.timeInShift = 0;
            }
        }
        return changes;
    }

    sf::Keyboard::Key ControlPlayerSystem::reverseKey(sf::Keyboard::Key key, bool reverse)
    {
        if (reverse) {
            if (key == sf::Keyboard::Key::Q)
                return sf::Keyboard::Key::D;
            if (key == sf::Keyboard::Key::D)
                return sf::Keyboard::Key::Q;
            if (key == sf::Keyboard::Key::Z)
                return sf::Keyboard::Key::S;
            if (key == sf::Keyboard::Key::S)
                return sf::Keyboard::Key::Z;
        }
        return key;
    }

} // namespace rtype
