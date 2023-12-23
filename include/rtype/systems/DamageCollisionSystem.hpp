//
// Created by edo on 12/12/23.
//

#ifndef R_TYPE_DAMAGECOLLISIONSYSTEM_HPP
#define R_TYPE_DAMAGECOLLISIONSYSTEM_HPP

#include "aecs/SystemBase.hpp"
#include "aecs/World.hpp"
#include "rtype/components/DamageCollisionComponent.hpp"
#include "rtype/components/HPComponent.hpp"
#include "rtype/components/PositionComponent.hpp"
#include "rtype/components/SpriteComponent.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
namespace rtype
{
    class DamageCollisionSystem : public aecs::ALogicSystem
    {
      public:
        DamageCollisionSystem(aecs::World &world,
                              const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
            ALogicSystem(world, entities,
                         {typeid(SpriteComponent), typeid(DamageCollisionComponent), typeid(HPComponent),
                          typeid(PositionComponent)})
        {
        }
        ~DamageCollisionSystem() override = default;

        static sf::Rect<float> getRect(const std::shared_ptr<aecs::Entity> &entity)
        {
            auto &sprite = entity->getComponent<SpriteComponent>();
            auto &position = entity->getComponent<PositionComponent>();
            sf::Rect rect = sprite.sprite.getGlobalBounds();
            rect.left = position.x;
            rect.top = position.y;
            float reduction = 0.3;
            rect.width *= 1 - reduction;
            rect.height *= 1 - reduction;
            rect.top = position.y + rect.height * (reduction / 2.0);
            rect.left = position.x + rect.width * (reduction / 2.0);
            return rect;
        }

        bool killed(const std::shared_ptr<aecs::Entity> &entity, const std::shared_ptr<aecs::Entity> &entity2)
        {
            auto &damage = entity->getComponent<DamageCollisionComponent>();
            auto &damage2 = entity2->getComponent<DamageCollisionComponent>();
            if (damage.team == damage2.team)
                return false;
            if (damage.type == DamageCollisionComponent::ObjectType::LG_BULLET &&
                damage2.type == DamageCollisionComponent::ObjectType::LG_BULLET)
                return true;
            return false;
        }

        void update(const std::vector<aecs::RenderInput> &inputs, float deltaTime) override
        {
            std::vector<std::shared_ptr<aecs::Entity>> entities;

            entities.reserve(_entitiesMap.size());
            for (auto &[_id, entity] : _entitiesMap) {
                entities.push_back(entity);
            }

            for (size_t i = 0; i < entities.size(); i++) {
                auto &entity = entities[i];
                auto &damage = entity->getComponent<DamageCollisionComponent>();
                auto &hp = entity->getComponent<HPComponent>();
                sf::Rect rect = getRect(entity);
                float dmg = 0;
                bool kll = false;
                for (size_t j = i + 1; j < entities.size(); j++) {
                    auto &entity2 = entities[j];
                    auto &damage2 = entity2->getComponent<DamageCollisionComponent>();
                    auto &hp2 = entity2->getComponent<HPComponent>();
                    sf::Rect rect2 = getRect(entity2);
                    if (rect.intersects(rect2) && damage.team != damage2.team) {
                        dmg += damage2.damage;
                        if (damage2.invulnerability == 0) {
                            hp2.hp -= damage.damage;
                            damage2.invulnerability = 5;
                        }
                        bool kill = killed(entity, entity2);
                        if (kill)
                            kll = true;
                        if (hp2.hp <= 0 || kill) {
                            _world.destroyEntity(*entity2);
                            entities.erase(entities.begin() + j);
                            j--;
                        }
                    }
                }
                if (dmg > 0 && damage.invulnerability == 0) {
                    hp.hp -= dmg;
                    damage.invulnerability = 5;
                }
                if (hp.hp <= 0 || kll) {
                    _world.destroyEntity(*entity);
                    entities.erase(entities.begin() + i);
                    i--;
                }
            }
        }
    };

} // namespace rtype

#endif // R_TYPE_DAMAGECOLLISIONSYSTEM_HPP
