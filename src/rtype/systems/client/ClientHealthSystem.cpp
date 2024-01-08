//
// Created by Clément Vandeville on 08/01/2024.
//

#include "rtype/systems/client/ClientHealthSystem.hpp"
#include "rtype/components/PlayerComponent.hpp"
#include "rtype/components/TagComponent.hpp"
#include "rtype/components/TextComponent.hpp"
#include "rtype/components/HPComponent.hpp"

namespace rtype {
    ClientHealthSystem::ClientHealthSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
            ALogicSystem(world, entities, {})
    {

    }

    aecs::EntityChanges ClientHealthSystem::update(aecs::UpdateParams &updateParams)
    {
        (void)updateParams;

        float currentHp;
        for (auto &[_id, entity] : _entitiesMap) {
            if (!entity->hasComponents({typeid(PlayerComponent), typeid(HPComponent)})) continue;

            auto &player = entity->getComponent<PlayerComponent>();
            if (player.playerId != 0) continue;

            auto &hp = entity->getComponent<HPComponent>();
            currentHp = hp.hp;
            break;
        }
        for (auto &[_id, entity] : _entitiesMap) {
            if (!entity->hasComponent<TagComponent>()) continue;

            auto &tag = entity->getComponent<TagComponent>();
            auto &text = entity->getComponent<TextComponent>();
            if (tag.tag != "healthText") continue;

            text.text.setString("Health: " + std::to_string(static_cast<int>(currentHp)));
        }

        return {};
    }
}

