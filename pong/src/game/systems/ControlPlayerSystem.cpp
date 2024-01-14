//
// Created by edo on 06/12/23.
//

#include "ControlPlayerSystem.hpp"

ControlPlayerSystem::ControlPlayerSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
        ALogicSystem(world, entities, {typeid(VelocityComponent), typeid(SpriteComponent), typeid(PlayerComponent), typeid(PositionComponent)})
{
}

aecs::EntityChanges ControlPlayerSystem::update(aecs::UpdateParams &updateParams)
{
    aecs::EntityChanges changes;
    for (auto &[_id, entity]: _entitiesMap) {
        if (!entity->hasComponent<PlayerComponent>())
            continue;

        const auto &position = entity->getComponent<PositionComponent>();
        auto &velocity = entity->getComponent<VelocityComponent>();
        auto &sprite = entity->getComponent<SpriteComponent>();
        auto &player = entity->getComponent<PlayerComponent>();

        if (player.isAI) continue;

        changes.editedEntities.insert(entity->getId());

        velocity.x = 0;
        velocity.y = 0;

        for (auto &[playerId, clientInputs]: updateParams.inputs) {
            if (playerId != player.playerId) { // Apply inputs only for the player we're controlling
                continue;
            }
            for (auto &input: clientInputs) {
                if (input == sf::Keyboard::Key::Z) {
                    velocity.y += -50;
                }
                if (input == sf::Keyboard::Key::S) {
                    velocity.y += 50;
                }
            }
        }
    }
    return changes;
}
