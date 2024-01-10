//
// Created by edo on 26/12/23.
//

#include "rtype/systems/server/PlayerOnConnectionSystem.hpp"
#include "rtype/components/PlayerComponent.hpp"

namespace rtype
{

    PlayerOnConnectionSystem::PlayerOnConnectionSystem(
        aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
        ALogicSystem(world, entities, {typeid(ClientAdressComponent)})
    {
    }

    aecs::EntityChanges PlayerOnConnectionSystem::update(aecs::UpdateParams &)
    {
        aecs::EntityChanges changes;
        while (!_deletedEntities.empty()) {
            changes.deletedEntities.insert(_deletedEntities[0]);
            _deletedEntities.erase(_deletedEntities.begin());
        }
        return {};
    }

    void PlayerOnConnectionSystem::onEntityAdded(const aecs::EntityPtr &entity)
    {
        if (entity->hasComponents(_componentsNeeded)) {
            _entitiesMap[entity->getId()] = entity;
        }
    }

    void PlayerOnConnectionSystem::onEntityModified(const aecs::EntityPtr &entity)
    {
        if (entity->hasComponents(_componentsNeeded)) {
            if (_entitiesMap.find(entity->getId()) == _entitiesMap.end()) {
                _entitiesMap[entity->getId()] = entity;
                entity->addComponent<PlayerComponent>();
                EntityFactory::toPlayer(*entity);
            }
        } else
            _entitiesMap.erase(entity->getId());
    }
    void PlayerOnConnectionSystem::onEntityRemoved(const aecs::EntityPtr &entity)
    {
        if (entity->hasComponents(_componentsNeeded)) {
            _entitiesMap.erase(entity->getId());
            _deletedEntities.push_back(entity->getId());
        }
    }

} // namespace rtype
