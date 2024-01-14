//
// Created by Clément Vandeville on 14/01/2024.
//

#include "AISystem.hpp"
#include "BallComponent.hpp"

AISystem::AISystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
        ALogicSystem(world, entities, {typeid(PositionComponent), typeid(VelocityComponent)})
{
}

sf::Vector2f AISystem::moveTo(sf::Vector2f from, sf::Vector2f to, float speed)
{
    float vectorX = to.x - from.x;
    float vectorY = to.y - from.y;

    float angle = atan2(vectorY, vectorX);
    float x = speed * cos(angle);
    float y = speed * sin(angle);

    if (vectorX == 0 && vectorY == 0)
        return {0, 0};
    return {x, y};
}

sf::Rect<float> AISystem::getRect(const std::shared_ptr<aecs::Entity> &entity)
{
    auto &sprite = entity->getComponent<SpriteComponent>();
    auto &position = entity->getComponent<PositionComponent>();
    sf::Rect rect = sprite.sprite.getGlobalBounds();

    return rect;
}

aecs::EntityChanges AISystem::update(aecs::UpdateParams &updateParams)
{
    (void)updateParams;
    aecs::EntityChanges changes;

    // Get ball position
    sf::Vector2f ballPos;
    for (auto &[_id, entity]: _entitiesMap) {
        if (!entity->hasComponent<BallComponent>())
            continue;
        auto &position = entity->getComponent<PositionComponent>();
        ballPos = {position.x, position.y};
    }

    for (auto &[_id, entity]: _entitiesMap) {
        if (!entity->hasComponent<PlayerComponent>())
            continue;
        auto &ai = entity->getComponent<PlayerComponent>();
        if (!ai.isAI) continue;

        sf::Rect rect = getRect(entity);

        auto &velocity = entity->getComponent<VelocityComponent>();
        auto &position = entity->getComponent<PositionComponent>();

        sf::Vector2f target = {position.x, ballPos.y - rect.height / 2};
        sf::Vector2f newVelocity = moveTo({position.x, position.y}, target, 20);
        velocity.x = newVelocity.x;
        velocity.y = newVelocity.y;
        changes.editedEntities.insert(entity->getId());
    }
    return {};
}