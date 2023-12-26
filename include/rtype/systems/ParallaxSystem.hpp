//
// Created by edo on 08/12/23.
//

#ifndef R_TYPE_PARALLAXSYSTEM_HPP
#define R_TYPE_PARALLAXSYSTEM_HPP

#include "aecs/SystemBase.hpp"
#include "aecs/World.hpp"
#include "rtype/components/ParallaxComponent.hpp"
#include "rtype/components/PositionComponent.hpp"
#include "rtype/components/SpriteComponent.hpp"
#include "rtype/components/VelocityComponent.hpp"
namespace rtype
{

    class ParallaxSystem : public aecs::ALogicSystem
    {
      public:
        ParallaxSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
            ALogicSystem(world, entities,
                         {typeid(ParallaxComponent), typeid(SpriteComponent), typeid(PositionComponent)})
        {
        }
        ~ParallaxSystem() override = default;

        aecs::EntityChanges update(aecs::UpdateParams &updateParams) override
        {
            aecs::EntityChanges changes;
            for (auto &[_id, entity] : _entitiesMap) {
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
    };

} // namespace rtype

#endif // R_TYPE_PARALLAXSYSTEM_HPP
