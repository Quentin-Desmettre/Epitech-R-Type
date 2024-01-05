//
// Created by edo on 12/12/23.
//

#include "rtype/systems/server/PowerSystem.hpp"

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
        aecs::EntityPtr &player = getPlayerId();
        sf::Rect playerRect = getRect(player);

        for (auto &[id, entity] : _entitiesMap) {
            if (!entity->hasComponent<PowerComponent>())
                continue;

            auto &power = entity->getComponent<PowerComponent>();
            auto &position = entity->getComponent<PositionComponent>();
            if (position.x < -100 || position.x > 1920 + 100 || position.y < -100 || position.y > 1080 + 100) {
                changes.deletedEntities.push_back(entity->getId());
                continue;
            }
            sf::Rect rect = getRect(entity);

            if (playerRect.intersects(rect)) {
                changes.deletedEntities.push_back(entity->getId());
                auto &playerComponent = player->getComponent<PlayerComponent>();
                if (power.isPowerUp)
                    playerComponent.timeLeftShootPowerUp = 10;
                else
                    playerComponent.timeLeftMovePowerDown = 10;
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

    aecs::EntityPtr &PowerSystem::getPlayerId()
    {
        for (auto &[id, entity] : _entitiesMap) {
            if (entity->hasComponent<PlayerComponent>())
                return entity;
        }
        return _entitiesMap.begin()->second;
    }

} // namespace rtype
