//
// Created by qdesmettre on 05/12/23.
//

#include "aecs/World.hpp"
#include "aecs/Entity.hpp"
#include <algorithm>
#include <iostream>

namespace aecs
{

    Entity &World::createEntity(size_t id)
    {
        // Create entity
        auto entity = std::make_shared<Entity>(*this, id);
        _entities[entity->getId()] = entity;

        // Notify systems
        onEntityAdded(entity);
        return *entity;
    }

    void World::destroyEntity(Entity &entity)
    {
        // Notify systems
        onEntityRemoved(_entities[entity.getId()]);

        // Remove entity
        _entities.erase(entity.getId());
    }

    void World::onEntityAdded(const EntityPtr &entity)
    {
        // Notify systems
        for (auto &[_, systemPair] : _systems)
            systemPair.first->onEntityAdded(entity);
        if (_renderSystem)
            _renderSystem->onEntityAdded(entity);
    }

    void World::onEntityRemoved(const EntityPtr &entity)
    {
        // Notify systems
        for (auto &[_, systemPair] : _systems)
            systemPair.first->onEntityRemoved(entity);
        if (_renderSystem)
            _renderSystem->onEntityRemoved(entity);
    }

    void World::onEntityChanged(const EntityPtr &entity)
    {
        // Notify systems
        for (auto &[_, systemPair] : _systems)
            systemPair.first->onEntityModified(entity);
        if (_renderSystem)
            _renderSystem->onEntityModified(entity);
    }

    void World::onEntityChanged(const aecs::Entity &entity)
    {
        onEntityChanged(_entities[entity.getId()]);
    }

    void World::sortSystems()
    {
        // Clear the sorted list
        _sortedSystems.clear();

        // Push each system in the sorted list
        for (auto &[_, systemPair] : _systems)
            _sortedSystems.emplace_back(systemPair.first.get(), systemPair.second);

        // Sort the list
        std::sort(_sortedSystems.begin(), _sortedSystems.end(), [](const auto &a, const auto &b) {
            return a.second < b.second;
        });
    }

    void World::update()
    {
        float deltaTime = clock.getElapsedTime().asSeconds() * 10;
        clock.restart();
        // Lock inputs
        std::lock_guard<std::mutex> lock(_renderInputsMutex);
        UpdateParams updateParams = {.inputs = getInputs(), .deltaTime = deltaTime, .entityChanges = {}};

        // Update systems
        for (auto &[system, _] : _sortedSystems) {
            auto changes = system->update(updateParams);

            // Update entity changes
            updateParams.entityChanges.deletedEntities.insert(updateParams.entityChanges.deletedEntities.end(),
                                                              changes.deletedEntities.begin(),
                                                              changes.deletedEntities.end());
            updateParams.entityChanges.editedEntities.insert(updateParams.entityChanges.editedEntities.end(),
                                                             changes.editedEntities.begin(),
                                                             changes.editedEntities.end());
        }

        // Delete entities
        for (auto &deletedEntityId : updateParams.entityChanges.deletedEntities) {
            _entities.erase(deletedEntityId);
        }

        // Clear
        // _renderInputs.clear();
        _tick++;
    }

    void World::render()
    {
        // Render system
        if (_renderSystem) {
            auto tmp = _renderSystem->render();
            {
                std::lock_guard<std::mutex> lock(_renderInputsMutex);
                setClientInputs(_clientId, tmp);
            }
        }
    }
} // namespace aecs
