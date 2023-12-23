//
// Created by edo on 12/12/23.
//

#ifndef R_TYPE_ANIMSYSTEM_HPP
#define R_TYPE_ANIMSYSTEM_HPP

#include "aecs/SystemBase.hpp"
#include "aecs/World.hpp"
#include "rtype/components/AnimComponent.hpp"
#include "rtype/components/SpriteComponent.hpp"
#include <iostream>
namespace rtype
{

    class AnimSystem : public aecs::ALogicSystem
    {
      public:
        AnimSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
            ALogicSystem(world, entities, {typeid(SpriteComponent), typeid(AnimComponent)})
        {
        }
        ~AnimSystem() override = default;

        void update(const std::vector<aecs::RenderInput> &inputs, float deltaTime) override
        {
            for (auto &[_id, entity] : _entitiesMap) {
                auto &sprite = entity->getComponent<SpriteComponent>();
                auto &anim = entity->getComponent<AnimComponent>();
                sf::IntRect rect = sprite.sprite.getTextureRect();

                anim.timeSinceLastFrame += deltaTime;
                while (anim.timeSinceLastFrame > anim.timeToWait) {
                    rect.left += rect.width;
                    anim.timeSinceLastFrame -= anim.timeToWait;
                }
                sprite.sprite.setTextureRect(rect);
            }
        }
    };

} // namespace rtype

#endif // R_TYPE_ANIMSYSTEM_HPP
