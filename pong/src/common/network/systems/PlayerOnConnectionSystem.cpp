//
// Created by edo on 26/12/23.
//

#include "PlayerOnConnectionSystem.hpp"
#include "PlayerComponent.hpp"
#include "PositionComponent.hpp"

PlayerOnConnectionSystem::PlayerOnConnectionSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
        ALogicSystem(world, entities, {typeid(ClientAddressComponent)})
{
}

aecs::EntityChanges PlayerOnConnectionSystem::update(aecs::UpdateParams &)
{
    aecs::EntityChanges changes;
    while (!_deletedEntities.empty()) {
        changes.deletedEntities.push_back(_deletedEntities[0]);
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
            entity->addComponent<PositionComponent>();

            auto &pos = entity->getComponent<PositionComponent>();

            std::cout << "Player connected " << _world.playersConnected << std::endl;
            if (_world.playersConnected < 2) {
                pos.x = 15;
            } else {
                pos.x = 1088 - 45;
            }
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
