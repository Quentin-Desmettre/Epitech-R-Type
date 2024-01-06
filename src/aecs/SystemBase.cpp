//
// Created by qdesmettre on 05/12/23.
//

#include "aecs/SystemBase.hpp"
#include "aecs/Entity.hpp"

namespace aecs
{
    ASystem::ASystem(aecs::World &world, const std::map<std::size_t, EntityPtr> &entities,
                     const std::vector<std::type_index> &componentsNeeded) :
        _world(world),
        _componentsNeeded(componentsNeeded)
    {
        for (auto &[_, entity] : entities)
            ASystem::onEntityAdded(entity);
    }

    void ASystem::onEntityAdded(const aecs::EntityPtr &entity)
    {
        if (entity->hasComponents(_componentsNeeded))
            _entitiesMap[entity->getId()] = entity;
    }

    void ASystem::onEntityRemoved(const aecs::EntityPtr &entity)
    {
        if (entity->hasComponents(_componentsNeeded))
            _entitiesMap.erase(entity->getId());
    }

    void ASystem::onEntityModified(const aecs::EntityPtr &entity)
    {
        if (entity->hasComponents(_componentsNeeded))
            _entitiesMap[entity->getId()] = entity;
        else
            _entitiesMap.erase(entity->getId());
    }

    // Render systems
    ARenderSystem::ARenderSystem(aecs::World &world, const std::map<std::size_t, EntityPtr> &entities,
                                 const std::vector<std::type_index> &componentsNeeded) :
        ASystem(world, entities, componentsNeeded)
    {
    }

    EntityChanges ARenderSystem::update(aecs::UpdateParams &)
    {
        throw std::runtime_error("IRenderSystem::update() should not be called");
    }

    // Logic systems
    ALogicSystem::ALogicSystem(aecs::World &world, const std::map<std::size_t, EntityPtr> &entities,
                               const std::vector<std::type_index> &componentsNeeded) :
        ASystem(world, entities, componentsNeeded)
    {
    }

    RenderInputs ALogicSystem::render()
    {
        throw std::runtime_error("IRenderSystem::render() should not be called");
    }
    bool ALogicSystem::isOpen() const
    {
        throw std::runtime_error("IRenderSystem::isOpen() should not be called");
    }
} // namespace aecs
