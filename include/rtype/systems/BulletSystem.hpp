//
// Created by edo on 10/12/23.
//

#ifndef R_TYPE_BULLETSYSTEM_HPP
#define R_TYPE_BULLETSYSTEM_HPP


#include "aecs/SystemBase.hpp"
#include "aecs/World.hpp"
#include "rtype/components/PositionComponent.hpp"
#include "rtype/components/VelocityComponent.hpp"
#include "rtype/components/SpriteComponent.hpp"
#include "rtype/components/BulletComponent.hpp"
#include <iostream>
namespace rtype
{

    class BulletSystem : public aecs::ALogicSystem
    {
      public:
        BulletSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
            ALogicSystem(world, entities, {typeid(SpriteComponent), typeid(BulletComponent)})
        {
        }
        ~BulletSystem() override = default;

        void update(const std::vector<aecs::RenderInput> &inputs, float deltaTime) override
        {
            for (auto &[_id, entity] : _entitiesMap) {
                auto &sprite = entity->getComponent<SpriteComponent>();
                sf::IntRect rect = sprite.sprite.getTextureRect();

                sprite.time += deltaTime;
                if (sprite.time > 1) {
                    rect.left += 20;
                    sprite.time -= 1;
                }
                rect.top = 0;
                sprite.sprite.setTextureRect(rect);

            }
        }
    };

} // namespace rtype

#endif // R_TYPE_BULLETSYSTEM_HPP
