//
// Created by qdesmettre on 27/12/23.
//

#include "rtype/systems/shared/ParallaxSystem.hpp"

namespace rtype
{

    ParallaxSystem::ParallaxSystem(aecs::World &world,
                                   const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
        ALogicSystem(world, entities, {typeid(ParallaxComponent), typeid(SpriteComponent), typeid(PositionComponent)})
    {
    }

    aecs::EntityChanges ParallaxSystem::update(aecs::UpdateParams &updateParams)
    {
        aecs::EntityChanges changes;
        for (auto &[_id, entity] : _entitiesMap) {
            std::cout << "ParallaxSystem" << std::endl;
            auto &parallax = entity->getComponent<ParallaxComponent>();
            auto &sprite = entity->getComponent<SpriteComponent>();
            auto &position = entity->getComponent<PositionComponent>();
            sf::IntRect rect = sprite.sprite.getTextureRect();
            sprite.time += updateParams.deltaTime;
            position.x -= parallax._speed.x * updateParams.deltaTime;
            if (position.x < -sprite._size.x) {
                position.x = 0;
            }
            sprite.sprite.setTextureRect(rect);
            changes.editedEntities.push_back(entity->getId());
        }
        return changes;
    }
} // namespace rtype
