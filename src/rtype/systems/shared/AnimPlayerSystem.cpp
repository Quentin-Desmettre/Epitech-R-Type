//
// Created by edo on 06/12/23.
//

#include "rtype/systems/shared/AnimPlayerSystem.hpp"

namespace rtype
{

    AnimPlayerSystem::AnimPlayerSystem(aecs::World &world,
                                       const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
        ALogicSystem(world, entities, {typeid(VelocityComponent), typeid(PlayerComponent), typeid(SpriteComponent)})
    {
    }

    aecs::EntityChanges AnimPlayerSystem::update(aecs::UpdateParams &updateParams)
    {
        (void)updateParams;
        for (auto &[_id, entity] : _entitiesMap) {
            auto &velocity = entity->getComponent<VelocityComponent>();
            auto &sprite = entity->getComponent<SpriteComponent>();
            sf::IntRect rect = sprite.sprite.getTextureRect();
            rect.top = 0;

            if (velocity.y < 0)
                rect.top = 32;
            if (velocity.y > 0)
                rect.top = 64;
            sprite.sprite.setTextureRect(rect);
        }
        return {};
    }

} // namespace rtype
