//
// Created by qdesmettre on 13/01/24.
//

#include "rtype/systems/shared/DifficultySystem.hpp"
#include "aecs/World.hpp"
#include "rtype/components/DifficultyComponent.hpp"
#include "rtype/components/TextComponent.hpp"

rtype::DifficultySystem::DifficultySystem(aecs::World &world,
                                          const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
    ALogicSystem(world, entities, {typeid(DifficultyComponent)})
{
}

aecs::EntityChanges rtype::DifficultySystem::update(aecs::UpdateParams &updateParams)
{
    if (_world.getIsServer())
        return increaseDifficulty(updateParams);
    return updateDifficultyText();
}

aecs::EntityChanges rtype::DifficultySystem::updateDifficultyText()
{
    for (auto &[id, entity] : _entitiesMap) {
        auto &difficulty = entity->getComponent<DifficultyComponent>().difficulty;
        auto &text = entity->getComponent<TextComponent>();
        std::string difficultyStr = "Difficulty: " + std::to_string(difficulty);
        text._text.setString(difficultyStr);
    }
    return {};
}

aecs::EntityChanges rtype::DifficultySystem::increaseDifficulty(aecs::UpdateParams &updateParams)
{
    aecs::EntityChanges changes;
    if (_entitiesMap.empty())
        changes.editedEntities.insert(EntityFactory::createDifficulty().getId());
    for (auto &[id, entity] : _entitiesMap) {
        auto &difficulty = entity->getComponent<DifficultyComponent>().difficulty;
        difficulty += updateParams.deltaTime * 0.1;
        changes.editedEntities.insert(entity->getId());
    }
    return changes;
}
