//
// Created by edo on 03/01/24.
//
#include "rtype/systems/shared/NodeMonsterSystem.hpp"
#include "rtype/components/NodeComponent.hpp"
#include "rtype/components/MonsterComponent.hpp"
#include "rtype/components/PositionComponent.hpp"
#include "rtype/components/VelocityComponent.hpp"
#include "rtype/components/SpriteComponent.hpp"
#include <cmath>

rtype::NodeMonsterSystem::
    NodeMonsterSystem(aecs::World &world,
                    const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
    ALogicSystem(world, entities, {typeid(NodeComponent), typeid(MonsterComponent), typeid(PositionComponent), typeid(VelocityComponent)})
{
}

void rtype::NodeMonsterSystem::follow(aecs::EntityPtr &entity1, float delta)
{
    auto &node1 = entity1->getComponent<NodeComponent>();
    auto &pos1 = entity1->getComponent<PositionComponent>();
    int i = int(positions.size()) -  int(node1.delay * 2 / 0.3);
    auto &sprite = entity1->getComponent<SpriteComponent>();
    sprite.zIndex = 40 - int(node1.delay);
    sprite.sprite.setRotation(node1.rotate);

    if (i - 1 < 0 || i >= int(positions.size())) {
        return;
    }
    sf::Vector2f prec = positions[i - 1];
    sf::Vector2f pos2 = positions[i];
    if (_world.getIsServer()) {
        float angle = atan2(prec.y - pos2.y, prec.x - pos2.x);
//        if (angle < 0)
//            angle += 2 * M_PI;
        node1.rotate = angle * 180 / M_PI;
    }

    pos1.x = pos2.x;
    pos1.y = pos2.y;
}


aecs::EntityChanges rtype::NodeMonsterSystem::update(aecs::UpdateParams &updateParams)
{
    aecs::EntityChanges changes;
    std::vector<std::shared_ptr<aecs::Entity>> entities;
    static float time = 0;
    time += updateParams.deltaTime;
    float offset = 0.3;

    entities.reserve(_entitiesMap.size());
    for (auto &[_id, entity] : _entitiesMap) {
        entities.push_back(entity);
    }
    for (std::size_t i = 0; i < entities.size(); i++) {
        auto &node = entities[i]->getComponent<NodeComponent>();
        if (node.delay != i)
            node.delay -= updateParams.deltaTime;
        if (node.delay < i || node.delay > 999)
            node.delay = i;
    }
    for (auto entity : entities) {
        auto &node = entity->getComponent<NodeComponent>();
        auto &monster = entity->getComponent<MonsterComponent>();
        follow(entity, updateParams.deltaTime);

        if (node.delay == 0) {
            monster._isShooting = true;
            wave(*entity, updateParams.deltaTime);
            if (time > offset) {
                auto &position = entity->getComponent<PositionComponent>();
                positions.emplace_back(position.x, position.y);
                time = 0;
            }
            continue;
        }
        if (int(node.delay) % 5 == 0)
            monster._isShooting = true;
        else
            monster._isShooting = false;
    }
    return changes;
}

void rtype::NodeMonsterSystem::onEntityRemoved(const aecs::EntityPtr &entity)
{

    if (entity->hasComponents(_componentsNeeded)) {
        auto &component = entity->getComponent<NodeComponent>();
        _dead.push_back(component.delay);
        _entitiesMap.erase(entity->getId());
    }
}
void rtype::NodeMonsterSystem::wave(aecs::Entity &entity, float delta)
{
        auto &pos = entity.getComponent<PositionComponent>();
        auto &vel = entity.getComponent<VelocityComponent>();
        auto &sprite = entity.getComponent<SpriteComponent>();
        sprite.zIndex = 40;
        static float dir = atan2(vel.y, vel.x);
        if (pos.x > 980) {
            vel.x = -20;
            vel.y = 0;
            return;
        }
        float angle = dir;
        if (pos.y > 400 || pos.y < 200)
            angle = dir + 0.1 * delta;
        dir = angle;
        if (angle > 2 * M_PI)
            angle -= 2 * M_PI;
        vel.x = cosf(angle) * 20;
        vel.y = sinf(angle) * 20;
        angle -= M_PI;
        sprite.sprite.setRotation(angle * 180 / M_PI);
}
