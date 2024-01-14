//
// Created by Clément Vandeville on 14/01/2024.
//

#include "ScoreSystem.hpp"
#include "PlayerComponent.hpp"
#include "TagComponent.hpp"
#include "TextComponent.hpp"

ScoreSystem::ScoreSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities)
: ALogicSystem(world, entities, {})
{
}

aecs::EntityChanges ScoreSystem::update(aecs::UpdateParams &updateParams)
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
        if (entity->hasComponent<TagComponent>()) {
            auto &tag = entity->getComponent<TagComponent>();
            auto &text = entity->getComponent<TextComponent>();

            std::cout << "score" << ai->score << std::endl;
            if (tag.tag == "scoreAI") {
                text._text.setString("Score: " + std::to_string(ai->score));
            }

            if (tag.tag == "scorePlayer") {
                text._text.setString("Score: " + std::to_string(player->score));
            }
        }
    }

    return changes;
}

