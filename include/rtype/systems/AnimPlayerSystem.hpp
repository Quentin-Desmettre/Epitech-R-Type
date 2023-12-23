//
// Created by edo on 06/12/23.
//

#ifndef R_TYPE_ANIMPLAYERSYSTEM_HPP
#define R_TYPE_ANIMPLAYERSYSTEM_HPP

#include "aecs/SystemBase.hpp"
#include "aecs/World.hpp"
#include "rtype/components/PlayerComponent.hpp"
#include "rtype/components/PositionComponent.hpp"
#include "rtype/components/SpriteComponent.hpp"
#include "rtype/components/VelocityComponent.hpp"
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
                rect.top = 0;

                if (velocity.y < 0)
                    rect.top = 32;
                if (velocity.y > 0)
                    rect.top = 64;
                sprite.sprite.setTextureRect(rect);
            }
        }
    };

} // namespace rtype

#endif // R_TYPE_ANIMPLAYERSYSTEM_HPP
