//
// Created by Clément Vandeville on 03/01/2024.
//

#include "rtype/systems/client/ClientScoreSystem.hpp"
#include "rtype/components/HPComponent.hpp"
#include "rtype/components/TagComponent.hpp"
#include "rtype/components/TextComponent.hpp"

namespace rtype {
    ClientScoreSystem::ClientScoreSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
            ALogicSystem(world, entities, {typeid(TagComponent)})
    {

    }

    aecs::EntityChanges ClientScoreSystem::update(aecs::UpdateParams &updateParams)
    {
        (void)updateParams;
        for (auto &[_id, entity] : _entitiesMap) {
            auto &tag = entity->getComponent<TagComponent>();
            if (tag.tag == "scoreText") {
                auto &text = entity->getComponent<TextComponent>();
                text.text.setString("Score: " + std::to_string(_score));
            }
        }
        return {};
    }

    void ClientScoreSystem::onEntityRemoved(const aecs::EntityPtr &entity)
    {
        if (!entity->hasComponent<MonsterComponent>())
            return;

        auto &monster = entity->getComponent<MonsterComponent>();
        auto &hp = entity->getComponent<HPComponent>();

        // TODO Les hp sont jamais a 0 :/
//        std::cout << hp.hp << std::endl;
//        if (hp.hp > 0) return;

        if (monster._lil)
            _score += 1;
        else
            _score += 10;
    }
}