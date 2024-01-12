//
// Created by edo on 11/01/24.
//
#include "rtype/systems/shared/BossSystem.hpp"
#include <algorithm>

namespace rtype
{
    BossSystem::BossSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
        ALogicSystem(world, entities, {typeid(PositionComponent), typeid(VelocityComponent), typeid(BossComponent)})
    {
    }
    void BossSystem::onEntityRemoved(const aecs::EntityPtr &entity)
    {
        if (entity->hasComponent<PlayerComponent>()&& std::find(players.begin(), players.end(), entity) != players.end()) {
            players.erase(std::remove(players.begin(), players.end(), entity), players.end());
        }
        ASystem::onEntityRemoved(entity);
    }
    void BossSystem::onEntityModified(const aecs::EntityPtr &entity)
    {
        if (entity->hasComponent<PlayerComponent>() && std::find(players.begin(), players.end(), entity) == players.end()) {
            players.push_back(entity);
        }
        ASystem::onEntityModified(entity);
    }
    aecs::EntityChanges BossSystem::update(aecs::UpdateParams &updateParams)
    {
        float averagePlayerPos = 0;
        for (auto &player : players) {
            auto &pos = player->getComponent<PositionComponent>();
            averagePlayerPos += pos.y;
        }
        averagePlayerPos /= static_cast<float>(players.size());
        for (auto &[_id, entity] : _entitiesMap) {
            auto &pos = entity->getComponent<PositionComponent>();
            auto &vel = entity->getComponent<VelocityComponent>();

            vel.y = std::max(std::min((averagePlayerPos - pos.y) * 0.1f, 10.f), -10.f);
            // reduce the y speed to stay on screen
            if (vel.x > 0) {
                vel.x -= 0.1f * updateParams.deltaTime;
            } else if (vel.x < 0) {
                vel.x += 0.1f * updateParams.deltaTime;
            }
            updateParams.entityChanges.editedEntities.insert(entity->getId());
        }
        return updateParams.entityChanges;
    }

}