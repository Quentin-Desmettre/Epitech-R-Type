//
// Created by edo on 06/12/23.
//

#ifndef R_TYPE_ANIMPLAYERSYSTEM_HPP
#define R_TYPE_ANIMPLAYERSYSTEM_HPP

#include "aecs/SystemBase.hpp"
#include "aecs/World.hpp"
#include "rtype/components/PositionComponent.hpp"
#include "rtype/components/PlayerComponent.hpp"
#include "rtype/components/VelocityComponent.hpp"
#include "rtype/components/SpriteComponent.hpp"
#include <iostream>

namespace rtype
{

    class AnimPlayerSystem : public aecs::ALogicSystem
    {
      public:
        AnimPlayerSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
            ALogicSystem(world, entities, {typeid(VelocityComponent), typeid(PlayerComponent), typeid(SpriteComponent)})
        {
        }
        ~AnimPlayerSystem() override = default;

        void update(const std::vector<aecs::RenderInput> &inputs, float deltaTime) override
        {
            for (auto &[_id, entity] : _entitiesMap) {
                auto &velocity = entity->getComponent<VelocityComponent>();
                auto &sprite = entity->getComponent<SpriteComponent>();
                sf::IntRect rect = sprite.sprite.getTextureRect();
                sprite.time += deltaTime;
                if (sprite.time > 1) {
                    rect.left += 96;
                    if (rect.left >= 96 * 7)
                        rect.left = 0;
                    sprite.time -= 1;
                }
                rect.top = 0;

                if (velocity.y < 0)
                    rect.top = 96;
                if (velocity.y > 0)
                    rect.top = 192;
                sprite.sprite.setTextureRect(rect);
            }
        }
    };

} // namespace rtype

#endif // R_TYPE_ANIMPLAYERSYSTEM_HPP
