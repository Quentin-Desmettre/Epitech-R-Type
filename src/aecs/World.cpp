//
// Created by qdesmettre on 05/12/23.
//

#include "aecs/World.hpp"
#include "aecs/Entity.hpp"
#include "shared/PacketBuilder.hpp"
#include <algorithm>
#include <iostream>
#include "rtype/systems/ServerInputsSystem.hpp"

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
        UpdateParams updateParams = {getInputs(), deltaTime};

        // Update systems
        for (auto &[system, _] : _sortedSystems) {
            system->update(updateParams);
            if (dynamic_cast<rtype::ServerInputsSystem *>(system))
                updateParams.inputs = getInputs();
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
                setClientInputs(0, tmp);
            }
        }
    }

    Entity &World::decodeNewEntity(std::vector<std::byte> &data)
    {
        PacketBuilder pb;
        int id;
        pb << data;
        pb >> id;
        Entity &entity = createEntity(id);
        ushort size;
        pb >> size;
        for (auto &i : decodeMap) {
            std::cout << i.first << std::endl;
        }
        while (pb) {
            uint componentId;
            pb >> componentId;
            // create a default component
            std::vector<std::byte> sub = pb.getSub();
            if (decodeMap.find(componentId) != decodeMap.end()) {
                std::cout << "componentId: " << componentId << std::endl;
                decodeMap.at(componentId)(entity, sub);
                break;
            }
        }
        // set the data of the server entity
        entity.decode(data);

        return entity;
    }
    void World::addDecodeMap(const std::type_info &type,
                             const std::function<void(aecs::Entity &, std::vector<std::byte>)> &map)
    {
        decodeMap[aecs::Entity::hashString(type.name())] = map;
    }
    bool World::getIsServer() const
    {
        return _isServer;
    }
} // namespace aecs
