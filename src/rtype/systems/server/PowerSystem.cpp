//
// Created by edo on 12/12/23.
//

#include "rtype/systems/server/PowerSystem.hpp"
#include "rtype/components/HPComponent.hpp"

namespace rtype
{
    PowerSystem::PowerSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
        ALogicSystem(world, entities, {typeid(SpriteComponent), typeid(PositionComponent)})
    {
    }

    void PowerSystem::onEntityAdded(const aecs::EntityPtr &entity)
    {
        if (entity->hasComponents(_componentsNeeded) &&
            (entity->hasComponent<PowerComponent>() || entity->hasComponent<PlayerComponent>()))
            _entitiesMap[entity->getId()] = entity;
    }

    void PowerSystem::onEntityRemoved(const aecs::EntityPtr &entity)
    {
        if (entity->hasComponents(_componentsNeeded) &&
            (entity->hasComponent<PowerComponent>() || entity->hasComponent<PlayerComponent>()))
            _entitiesMap.erase(entity->getId());
    }

    void PowerSystem::onEntityModified(const aecs::EntityPtr &entity)
    {
        if (entity->hasComponents(_componentsNeeded) &&
            (entity->hasComponent<PowerComponent>() || entity->hasComponent<PlayerComponent>()))
            _entitiesMap[entity->getId()] = entity;
        else
            _entitiesMap.erase(entity->getId());
    }

    aecs::EntityChanges PowerSystem::update(aecs::UpdateParams &updateParams)
    {
        (void)updateParams;
        aecs::EntityChanges changes;

        if (_entitiesMap.empty())
            return changes;
        auto players = getPlayers();
        for (auto &player : players) {
        sf::Rect playerRect = getRect(player);

        for (auto &[id, entity] : _entitiesMap) {
            if (!entity->hasComponent<PowerComponent>())
                continue;

            auto &power = entity->getComponent<PowerComponent>();
            auto &position = entity->getComponent<PositionComponent>();

            // Remove power if out of screen
            if (position.x < -100 || position.x > 1920 + 100 || position.y < -100 || position.y > 1080 + 100) {
                changes.deletedEntities.insert(entity->getId());
                continue;
            }

            // Check if player is in range
            sf::Rect rect = getRect(entity);
            if (!playerRect.intersects(rect))
                continue;
            changes.deletedEntities.insert(entity->getId());
            auto &playerComponent = player->getComponent<PlayerComponent>();
            auto &hp = player->getComponent<HPComponent>();
            switch (power.type) {
                case PowerComponent::PowerType::DOUBLE_SHOT:
                    playerComponent.timeLeftShootPowerUp = 10;
                    break;
                case PowerComponent::PowerType::INVERSE_DIR:
                    playerComponent.timeLeftMovePowerDown = 5;
                    break;
                case PowerComponent::PowerType::HEALTH_PACK:
                    hp.hp = std::min(hp.hp + 25, hp.maxHp);
                    break;
                default:
                    break;
            }
        }
        }

        return changes;
    }

    sf::Rect<float> PowerSystem::getRect(const std::shared_ptr<aecs::Entity> &entity)
    {
        auto &sprite = entity->getComponent<SpriteComponent>();
        auto &position = entity->getComponent<PositionComponent>();
        sf::Rect rect = sprite.sprite.getGlobalBounds();
        rect.left = position.x;
        rect.top = position.y;
        float reduction = 0.3;
        rect.width *= 1 - reduction;
        rect.height *= 1 - reduction;
        rect.top = position.y + rect.height * (reduction / 2.0);
        rect.left = position.x + rect.width * (reduction / 2.0);
        return rect;
    }

    std::vector<aecs::EntityPtr> PowerSystem::getPlayers()
    {
        std::vector<aecs::EntityPtr> players;

        for (auto &[id, entity] : _entitiesMap) {
            if (entity->hasComponent<PlayerComponent>())
                players.push_back(entity);
        }
        return players;
    }

} // namespace rtype
