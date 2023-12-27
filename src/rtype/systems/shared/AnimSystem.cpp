//
// Created by edo on 12/12/23.
//

#include "rtype/systems/shared/AnimSystem.hpp"

namespace rtype
{
    AnimSystem::AnimSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
        ALogicSystem(world, entities, {typeid(SpriteComponent), typeid(AnimComponent)})
    {
    }

    aecs::EntityChanges AnimSystem::update(aecs::UpdateParams &updateParams)
    {
        for (auto &[_id, entity] : _entitiesMap) {
            auto &sprite = entity->getComponent<SpriteComponent>();
            auto &anim = entity->getComponent<AnimComponent>();
            sf::IntRect rect = sprite.sprite.getTextureRect();

            anim.timeSinceLastFrame += updateParams.deltaTime;
            while (anim.timeSinceLastFrame > anim.timeToWait) {
                rect.left += rect.width;
                anim.timeSinceLastFrame -= anim.timeToWait;
            }
            sprite.sprite.setTextureRect(rect);
        }
        return {};
    }

} // namespace rtype
