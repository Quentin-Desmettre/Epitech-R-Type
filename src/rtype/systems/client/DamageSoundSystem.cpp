//
// Created by edo on 29/12/23.
//

#include "rtype/systems/client/DamageSoundSystem.hpp"
#include "aecs/SystemBase.hpp"
//#include "rtype/components/PlayerComponent.hpp"
#include "rtype/components/MonsterComponent.hpp"
#include "rtype/components/DamageCollisionComponent.hpp"

rtype::DamageSoundSystem::DamageSoundSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities)
    : ALogicSystem(
            world, entities,
            {typeid(DamageCollisionComponent)})
{
    _soundBuffer.loadFromFile("assets/sounds/damage.ogg");
    _sound.setBuffer(_soundBuffer);
}

aecs::EntityChanges rtype::DamageSoundSystem::update(unused aecs::UpdateParams &updateParams)
{
    for (auto &[_id, entity] : _entitiesMap) {
        auto &damage = entity->getComponent<DamageCollisionComponent>();

        if (damage.damaged) {
            if (entity->hasComponent<MonsterComponent>()) {
                _sound.setPitch(2);
                _sound.setVolume(50);
            } else {
                _sound.setPitch(1);
                _sound.setVolume(100);
            }
            _sound.play();
            damage.damaged = false;
        }

    }
    return {};
}
void rtype::DamageSoundSystem::onEntityRemoved(const aecs::EntityPtr &entity)
{
    if (entity->hasComponents(_componentsNeeded)) {
        if (entity->hasComponent<MonsterComponent>()) {
            _sound.setVolume(50);
            _sound.setPitch(3);
            _sound.play();
        }
        _entitiesMap.erase(entity->getId());
    }
}
