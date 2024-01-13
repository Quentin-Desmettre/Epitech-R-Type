//
// Created by edo on 12/12/23.
//

#include "rtype/systems/shared/InvulSystem.hpp"
#include "rtype/components/InvulComponent.hpp"

namespace rtype
{

    InvulSystem::InvulSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
        ALogicSystem(world, entities, {typeid(SpriteComponent), typeid(DamageCollisionComponent), typeid(InvulComponent)})
    {
    }

    aecs::EntityChanges InvulSystem::update(aecs::UpdateParams &updateParams)
    {
        aecs::EntityChanges changes;
        for (auto &[_id, entity] : _entitiesMap) {
            auto &sprite = entity->getComponent<SpriteComponent>();
            auto &damage = entity->getComponent<DamageCollisionComponent>();
            if (damage.invulnerability > 0) {
                changes.editedEntities.insert(entity->getId());
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

} // namespace rtype
