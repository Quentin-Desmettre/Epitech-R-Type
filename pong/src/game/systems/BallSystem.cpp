//
// Created by Clément Vandeville on 14/01/2024.
//

#include "BallSystem.hpp"
#include "BallComponent.hpp"
#include <cmath>
const double PI = 3.14159265358979323846;

BallSystem::BallSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
        ALogicSystem(world, entities, {typeid(PositionComponent), typeid(VelocityComponent)})
{
}

sf::Rect<float> BallSystem::getRect(const std::shared_ptr<aecs::Entity> &entity)
{
    auto &sprite = entity->getComponent<SpriteComponent>();
    auto &position = entity->getComponent<PositionComponent>();
    sf::Rect rect = sprite.sprite.getGlobalBounds();

    return rect;
}

sf::Vector2f BallSystem::moveTo(sf::Vector2f from, sf::Vector2f to, float speed)
{
    float vectorX = to.x - from.x;
    float vectorY = to.y - from.y;

    float angle = std::atan2(vectorY, vectorX);
    float x = speed * std::cos(angle);
    float y = speed * std::sin(angle);

    if (vectorX == 0 && vectorY == 0)
        return {0, 0};
    return {x, y};
}

aecs::EntityChanges BallSystem::update(aecs::UpdateParams &updateParams)
{
    aecs::EntityChanges changes;

    PlayerComponent *player = nullptr;
    PlayerComponent *ai = nullptr;

    for (auto &[_id, entity]: _entitiesMap) {
        if (entity->hasComponent<PlayerComponent>()) {
            auto &component = entity->getComponent<PlayerComponent>();
            if (!component.isAI)
                player = &component;
        }
        if (entity->hasComponent<PlayerComponent>()) {
            auto &component = entity->getComponent<PlayerComponent>();
            if (component.isAI)
                ai = &component;
        }
    }

    for (auto &[_id, entity]: _entitiesMap) {

        // Make the ball move if there is a player
        if (entity->hasComponent<BallComponent>()) {
            auto &velocity = entity->getComponent<VelocityComponent>();
            if (velocity.x == 0 && velocity.y == 0) {
                changes.editedEntities.insert(entity->getId());
                velocity.x = -30;
            }
        }

        // Ball bounce on walls
        if (entity->hasComponent<BallComponent>()) {
            auto &position = entity->getComponent<PositionComponent>();
            auto &velocity = entity->getComponent<VelocityComponent>();
            sf::Rect rect = getRect(entity);

            if (position.y < 0) {
                position.y = 0;
                velocity.y = -velocity.y;
            }
            if (position.y > 640 - rect.height) {
                position.y = 640 - rect.height;
                velocity.y = -velocity.y;
            }

            if (position.x < 0) {
                ai->score++;
                velocity.y = 0;
                position.x = 544;
                position.y = 320;
            }
            if (position.x > 1088) {
                player->score++;
                velocity.y = 0;
                position.x = 544;
                position.y = 320;
            }

            // Ball bounce on players
            for (auto &[_id2, entity2]: _entitiesMap) {
                if (entity2->hasComponent<PlayerComponent>()) {
                    auto &position2 = entity2->getComponent<PositionComponent>();
                    sf::Rect rect2 = getRect(entity2);
                    if (rect.intersects(rect2)) {
                        position.x = (position2.x + (velocity.x > 0 ? -rect2.width - 10 : rect2.width + 10));
                        velocity.x = -velocity.x;

                        double intersectY = position.y - position2.y - rect2.height / 2;
                        double relativeIntersectY = intersectY + rect2.height / 2;
                        double normalizedRelativeIntersectionY = relativeIntersectY / (rect2.height / 2);
                        double bounceAngle = normalizedRelativeIntersectionY * (75.0 * PI / 180.0);

                        velocity.y = 30 * sin(bounceAngle);
                    }
                }
            }
        }
    }

    return changes;
}