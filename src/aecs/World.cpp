//
// Created by qdesmettre on 05/12/23.
//

#include "aecs/World.hpp"
#include "aecs/Entity.hpp"
#include "rtype/systems/server/ServerInputsSystem.hpp"
#include "shared/PacketBuilder.hpp"
#include <algorithm>
#include <iostream>

namespace aecs
{
    World::World(bool isServer) :
        _isServer(isServer)
    {
    }

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
            auto it = _entities.find(deletedEntityId);
            if (it != _entities.end())
                destroyEntity(*it->second);
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

    void World::decodeNewEntity(Entity &entity, const std::vector<std::byte> &data)
    {
        PacketBuilder pb;

        pb << data;
        // ushort size;
        // pb >> size;
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

    void World::setClientId(unsigned int clientId)
    {
        _clientId = clientId;
    }

    unsigned int World::getClientId() const
    {
        return _clientId;
    }

    void World::setTick(unsigned int tick)
    {
        _tick = tick;
    }

    unsigned int World::getTick() const
    {
        return _tick;
    }

    EntityPtr World::getEntity(std::size_t id) const
    {
        auto it = _entities.find(id);

        if (it == _entities.end())
            return nullptr;
        return it->second;
    }

    std::vector<std::byte> World::serialize() const
    {
        PacketBuilder packet;

        packet << static_cast<std::uint32_t>(_tick);
        packet << static_cast<std::uint32_t>(_entities.size());
        for (auto &[id, entity] : _entities) {
            const auto &encoded = entity->encode();
            packet.add(encoded.data(), encoded.size());
        }

        return packet.getData();
    }

    ServerInputs World::getInputs(unsigned int tick) const
    {
        if (tick == (unsigned)(-1))
            tick = _tick;
        auto it = _renderInputs.find(tick);

        if (it == _renderInputs.end())
            return {};
        return it->second;
    }

    ClientInputs World::getClientInputs(unsigned int clientId, std::size_t tick) const
    {
        if (tick == (std::size_t)(-1))
            tick = _tick;
        auto it = _renderInputs.find(tick);

        if (it == _renderInputs.end())
            return {};
        auto it2 = it->second.find(clientId);
        if (it2 == it->second.end())
            return {};
        return it2->second;
    }

    void World::setInputs(const aecs::ServerInputs &inputs)
    {
        for (auto it = _renderInputs.begin(); it != _renderInputs.end();) {
            if (it->first < _tick - 600)
                it = _renderInputs.erase(it);
            else
                ++it;
        }
        if (_renderInputs.find(_tick) == _renderInputs.end())
            _renderInputs[_tick] = inputs;
        else
            _renderInputs[_tick].insert(inputs.begin(), inputs.end());
    }

    void World::setClientInputs(unsigned clientId, const ClientInputs &inputs)
    {
        for (auto it = _renderInputs.begin(); it != _renderInputs.end();) {
            if (it->first < _tick - 600)
                it = _renderInputs.erase(it);
            else
                ++it;
        }
        if (_renderInputs.find(_tick) == _renderInputs.end())
            _renderInputs[_tick] = {{clientId, inputs}};
        else
            _renderInputs[_tick].insert({clientId, inputs});
    }

    void World::load(const aecs::World::EncodedGameState &entities)
    {
        // Load entities
        for (const auto &[id, encodedEntity] : entities.encodedEntities) {
            auto entity = getEntity(id);

            if (!entity) {
                decodeNewEntity(createEntity(id), encodedEntity);
            } else
                entity->decode(encodedEntity);
        }

        // Set tick
        _tick = entities.tick;
    }

} // namespace aecs
