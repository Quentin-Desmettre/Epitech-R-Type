//
// Created by qdesmettre on 13/01/24.
//

#include "rtype/systems/shared/DifficultySystem.hpp"
#include "aecs/World.hpp"
#include "rtype/components/DifficultyComponent.hpp"
#include "rtype/components/TextComponent.hpp"
#include "rtype/components/MonsterComponent.hpp"
#include "rtype/components/BossComponent.hpp"
#include <iomanip>
#include <sstream>
#include "rtype/components/NodeComponent.hpp"
#include "rtype/components/HPComponent.hpp"

rtype::DifficultySystem::DifficultySystem(aecs::World &world,
                                          const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
    ALogicSystem(world, entities, {})
{
    for (auto &[_, entity] : entities)
        DifficultySystem::onEntityAdded(entity);
}

void rtype::DifficultySystem::onEntityAdded(const aecs::EntityPtr &entity)
{
    // For node component
    if (entity->hasComponent<NodeComponent>()) {
        _nodes.insert(entity);
    }

    // For difficulty component
    if (entity->hasComponent<DifficultyComponent>()) {
        _difficulty = entity;
        EntityFactory::setDifficulty(&entity->getComponent<DifficultyComponent>().difficulty);
    }
}

void rtype::DifficultySystem::onEntityRemoved(const aecs::EntityPtr &entity)
{
    // For node component
    if (entity->hasComponent<NodeComponent>()) {
        _nodes.erase(entity);
    }

    // For difficulty component
    if (entity->hasComponent<DifficultyComponent>()) {
        _difficulty = nullptr;
        EntityFactory::setDifficulty(nullptr);
    }
}

void rtype::DifficultySystem::onEntityModified(const aecs::EntityPtr &entity)
{
    // For node component
    if (entity->hasComponent<NodeComponent>()) {
        _nodes.insert(entity);
    }

    // For difficulty component
    if (entity->hasComponent<DifficultyComponent>()) {
        _difficulty = entity;
        EntityFactory::setDifficulty(&entity->getComponent<DifficultyComponent>().difficulty);
    }
}

aecs::EntityChanges rtype::DifficultySystem::update(aecs::UpdateParams &updateParams)
{
    aecs::EntityChanges changes;

    if (!_difficulty)
        changes.editedEntities.insert(EntityFactory::createDifficulty().getId());
    if (_world.getIsServer())
        return increaseDifficulty(updateParams, changes);
    return updateDifficultyText(changes);
}

aecs::EntityChanges rtype::DifficultySystem::updateDifficultyText(aecs::EntityChanges changes)
{
    auto &difficulty = _difficulty->getComponent<DifficultyComponent>().difficulty;
    auto &text = _difficulty->getComponent<TextComponent>();
    std::stringstream difficultyStr;
    difficultyStr << "Difficulty: " << std::fixed << std::setprecision(2) << difficulty;
    text._text.setString(difficultyStr.str());
    return changes;
}

aecs::EntityChanges rtype::DifficultySystem::increaseDifficulty(aecs::UpdateParams &updateParams, aecs::EntityChanges changes)
{
    float &difficulty = _difficulty->getComponent<DifficultyComponent>().difficulty;
    const float baseDifficulty = difficulty;
    std::size_t deletedNodes = 0;

    // When a monster / boss is killed, increase difficulty
    for (auto &id: updateParams.entityChanges.deletedEntities) {
        auto entity = _world.getEntity(id);
        auto *hp = entity->safeGetComponent<HPComponent>();
        auto *monster = entity->safeGetComponent<MonsterComponent>();
        auto *boss = entity->safeGetComponent<BossComponent>();
        auto *node = entity->safeGetComponent<NodeComponent>();

        // In case of deletion out of screen
        if (!hp || hp->hp > 0)
            continue;

        // Increase difficulty
        if (monster) {
            difficulty += monster->_lil ? DIFFICULTY_INCREASE_LIL_MONSTER : DIFFICULTY_INCREASE_BIG_MONSTER;
        } else if (boss) {
            difficulty += DIFFICULTY_INCREASE_BOSS;
        } else if (node) {
            deletedNodes++; // Count deleted nodes
        }
    }

    // Increase difficulty if all nodes are deleted
    if (deletedNodes >= _nodes.size() && deletedNodes > 0) {
        difficulty += DIFFICULTY_INCREASE_BOSS;
    }

    if (baseDifficulty != difficulty)
        changes.editedEntities.insert(_difficulty->getId());
    return changes;
}
