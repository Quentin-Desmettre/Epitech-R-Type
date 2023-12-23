//
// Created by qdesmettre on 05/12/23.
//

#include "aecs/World.hpp"
#include "aecs/Entity.hpp"
#include <algorithm>
#include <iostream>

namespace aecs
{

    Entity &World::createEntity()
    {
        // Create entity
        auto entity = std::make_shared<Entity>(*this);
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
        UpdateParams updateParams = {getInputs(), deltaTime};

        // Update systems
        for (auto &[system, _] : _sortedSystems)
            system->update(updateParams);

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
                setClientInputs(0, tmp);
            }
        }
    }
} // namespace aecs
