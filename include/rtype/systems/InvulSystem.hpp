//
// Created by edo on 12/12/23.
//

#ifndef R_TYPE_INVULSYSTEM_HPP
#define R_TYPE_INVULSYSTEM_HPP

#include "aecs/SystemBase.hpp"
#include "aecs/World.hpp"
#include "rtype/components/DamageCollisionComponent.hpp"
#include "rtype/components/SpriteComponent.hpp"
namespace rtype
{

    class InvulSystem : public aecs::ALogicSystem
    {
      public:
        InvulSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
            ALogicSystem(world, entities, {typeid(SpriteComponent), typeid(DamageCollisionComponent)})
        {
        }
        ~InvulSystem() override = default;

        aecs::EntityChanges update(const aecs::UpdateParams &updateParams) override
        {
            aecs::EntityChanges changes;
            for (auto &[_id, entity] : _entitiesMap) {
                auto &sprite = entity->getComponent<SpriteComponent>();
                auto &damage = entity->getComponent<DamageCollisionComponent>();
                if (damage.invulnerability > 0) {
                    changes.editedEntities.push_back(entity->getId());
                    damage.invulnerability -= updateParams.deltaTime;
                    if (damage.invulnerability < 0)
                        damage.invulnerability = 0;
                    sf::Color color = sprite.sprite.getColor();
                    if (int(damage.invulnerability * 2) % 2 == 0) {
                        color.a = 150;
                    } else {
                        color.a = 255;
                    }
                    sprite.sprite.setColor(color);
                } else {
                    sf::Color color = sprite.sprite.getColor();
                    color.a = 255;
                    sprite.sprite.setColor(color);
                }
            }
            return changes;
        }
    };

} // namespace rtype

#endif // R_TYPE_INVULSYSTEM_HPP
