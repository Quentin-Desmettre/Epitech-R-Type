//
// Created by qdesmettre on 05/12/23.
//

#include "rtype/systems/shared/PhysicsSystem.hpp"
#include "rtype/components/CollidableComponent.hpp"
#include "rtype/components/PlayerComponent.hpp"

namespace rtype
{

    PhysicsSystem::PhysicsSystem(aecs::World &world,
                                 const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
        ALogicSystem(world, entities, {})
    {
        for (auto &[_, entity] : entities) {
            PhysicsSystem::onEntityAdded(entity);
        }
    }

    aecs::EntityChanges PhysicsSystem::update(aecs::UpdateParams &updateParams)
    {
        aecs::EntityChanges changes;
        for (auto &[_id, entity] : _movableEntities) {
            auto &position = entity->getComponent<PositionComponent>();
            auto &velocity = entity->getComponent<VelocityComponent>();
            auto &size = entity->getComponent<SpriteComponent>()._size;
            sf::Vector2f oldPosition = {position.x, position.y};
            float dx = velocity.x * updateParams.deltaTime;
            float dy = velocity.y * updateParams.deltaTime;

            // Move
            position.x += dx;
            position.y += dy;

            // Don't move player too far
            if (entity->hasComponent<PlayerComponent>() && isOutOfScreen(position, size)) {
                position.x -= dx;
                position.y -= dy;
            }

            // Check if need to erase
            if (position.deleteIfOutOfScreen && isOutOfScreen(position, size)) {
                changes.deletedEntities.push_back(_id);
                continue;
            }
            // Check collisions
            // if same level of priority, or if current entity has a lower priority than the other
            //      don't move current entity
            // if current entity has a higher priority than the other
            //      move the other entity in the same direction
            if (!entity->hasComponent<CollidableComponent>())
                continue;
            int priority = entity->getComponent<CollidableComponent>().getPriority();
            auto collisions = getCollisions(entity);
            for (auto &[entityOther, direction]: collisions) {
                int priorityOther = entityOther->getComponent<CollidableComponent>().getPriority();
                if (priority <= priorityOther) {
                    position.x = oldPosition.x;
                    position.y = oldPosition.y;
                    break;
                } else if (priority > priorityOther) {
                    auto &positionOther = entityOther->getComponent<PositionComponent>();
                    // Move over Y only if collision is on LEFT or RIGHT
                    if (direction & LEFT || direction & RIGHT)
                        positionOther.x += dx;
                    // Move over X only if collision is on TOP or BOTTOM
                    if (direction & TOP || direction & BOTTOM)
                        positionOther.y += dy;
                }
            }
        }
        return changes;
    }

    PhysicsSystem::CollisionDirection PhysicsSystem::getCollisionDirection(const aecs::EntityPtr &entity1, const aecs::EntityPtr &entity2)
    {
        if (!entity1->hasComponents({typeid(CollidableComponent)}) ||
            !entity2->hasComponents({typeid(CollidableComponent)}))
            return NONE;

        auto &position1 = entity1->getComponent<PositionComponent>();
        auto &sprite1 = entity1->getComponent<SpriteComponent>();
        auto &position2 = entity2->getComponent<PositionComponent>();
        auto &sprite2 = entity2->getComponent<SpriteComponent>();

        sf::Rect<float> rect1(position1.x, position1.y, sprite1._size.x, sprite1._size.y);
        sf::Rect<float> rect2(position2.x, position2.y, sprite2._size.x, sprite2._size.y);
        int direction = NONE;
        if (rect1.intersects(rect2)) {
            if (rect1.top < rect2.top)
                direction |= TOP;
            if (rect1.top > rect2.top)
                direction |= BOTTOM;
            if (rect1.left < rect2.left)
                direction |= LEFT;
            if (rect1.left > rect2.left)
                direction |= RIGHT;
        }
        return static_cast<CollisionDirection>(direction);
    }

    std::vector<std::pair<aecs::EntityPtr, PhysicsSystem::CollisionDirection>> PhysicsSystem::getCollisions(const aecs::EntityPtr &entity)
    {
        std::vector<std::pair<aecs::EntityPtr, CollisionDirection>> collisions;
        CollisionDirection direction;
        for (auto &[_id, entity2] : _collidableEntities) {
            if (entity->getId() == _id)
                continue;
            direction = getCollisionDirection(entity, entity2);
            if (direction != NONE)
                collisions.emplace_back(entity2, direction);
        }
        return collisions;
    }

    bool PhysicsSystem::isOutOfScreen(const PositionComponent &position, const sf::Vector2f &size)
    {
        return position.x + size.x < 0 || position.x > 1088 || position.y + size.y < 0 || position.y > 640;
    }


    void PhysicsSystem::onEntityAdded(const aecs::EntityPtr &entity)
    {
        // We need position and sprite for physics
        if (!entity->hasComponents({typeid(PositionComponent), typeid(SpriteComponent)}))
            return;
        if (entity->hasComponent<VelocityComponent>())
            _movableEntities[entity->getId()] = entity;
        if (entity->hasComponent<CollidableComponent>())
            _collidableEntities[entity->getId()] = entity;
    }

    void PhysicsSystem::onEntityRemoved(const aecs::EntityPtr &entity)
    {
        _movableEntities.erase(entity->getId());
        _collidableEntities.erase(entity->getId());
    }

    void PhysicsSystem::onEntityModified(const aecs::EntityPtr &entity)
    {
        onEntityRemoved(entity);
        onEntityAdded(entity);
    }
} // namespace rtype