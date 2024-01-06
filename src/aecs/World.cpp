//
// Created by qdesmettre on 05/12/23.
//

#include "aecs/World.hpp"
#include "aecs/Entity.hpp"
#include "aecs/MenuInputSystem.hpp"
#include "rtype/systems/server/ServerInputsSystem.hpp"
#include "shared/PacketBuilder.hpp"
#include <algorithm>
#include <iostream>

namespace aecs
{
    World::World(bool isServer, int ac, char **av) :
        _isServer(isServer),
        _argParser(ac, av)
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
        if (_entities.find(entity.getId()) == _entities.end()) {
            std::cerr << "connait pas " << entity.getId() << std::endl;
            return;
        }
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
        UpdateParams updateParams;
        // Lock inputs
        {
            std::lock_guard<std::mutex> lock(_renderInputsMutex);
            updateParams = {
                .inputs = _isServer ? getInputs() : popInputs(), .mouseInputs = _mouseInputs, .deltaTime = deltaTime, .entityChanges = {}};
        }

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
                setClientInputs(_clientId, tmp.first);
                _mouseInputs = tmp.second;
            }
        }
    }

    void World::decodeNewEntity(Entity &entity, const std::vector<std::byte> &data)
    {
        PacketBuilder pb;

        pb << data;
        // ushort size;
        // pb >> size;
        while (pb) {
            uint componentId;
            pb >> componentId;
            // create a default component
            std::vector<std::byte> sub = pb.getSub();
            if (decodeMap.find(componentId) != decodeMap.end()) {
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

    ServerInputs World::popInputs()
    {
        if (_renderInputs.find(0) == _renderInputs.end())
            return {};
        ServerInputs inputs = _renderInputs[0];

        _renderInputs.erase(0);
        return inputs;
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

    void World::setClientInputs(unsigned clientId, const ClientInputs &inputs)
    {
        unsigned tick = _isServer ? _tick : 0;

        for (auto it = _renderInputs.begin(); it != _renderInputs.end();) {
            if (it->first < tick - 600)
                it = _renderInputs.erase(it);
            else
                ++it;
        }
        if (_renderInputs.find(tick) == _renderInputs.end())
            _renderInputs[tick] = {{clientId, inputs}};
        else
            _renderInputs[tick].insert({clientId, inputs});
    }

    void World::load(const aecs::World::EncodedGameState &entities)
    {
        // Load entities
        for (const auto &[id, encodedEntity] : entities.encodedEntities) {
            auto entity = getEntity(id);

            if (encodedEntity.empty()) {
                if (entity)
                    destroyEntity(*entity);
                continue;
            }
            if (!entity)
                decodeNewEntity(createEntity(id), encodedEntity);
            else
                entity->decode(encodedEntity);
        }

        // Set tick
        _tick = entities.tick;
    }

    std::string World::getIp() {
        return _argParser.getIp();
    }

    unsigned short World::getPort() {
        return _argParser.getPort();
    }

    void World::registerSystem(const std::shared_ptr<ISystem> &system, int priority)
    {
        _systems[typeid(*system)] = {system, priority};
        sortSystems();
    }

    int World::addMenu(const Menu &menu, int id)
    {
        if (id == -1)
            id = _menus.size();
        _menus.emplace(id, menu);
        return id;
    }

    void World::registerSystem(const std::shared_ptr<ISystem> &system, int priority)
    {
        _systems[typeid(*system)] = {system, priority};
        sortSystems();
    }

    int World::addMenu(const Menu &menu, int id)
    {
        if (id == -1)
            id = _menus.size();
        _menus.emplace(id, menu);
        return id;
    }

    void World::goToMenu(int id)
    {
        if (_currentMenu == id)
            return;
        if (_menus.find(id) == _menus.end())
            return;
        _currentMenu = id;
        this->_systems.clear();
        this->_sortedSystems.clear();
        this->_entities.clear();
        this->_renderInputs.clear();

        _menus.at(id)._setup();

        this->registerSystem<MenuInputSystem>(0);


//        for (auto &button : _menus.at(id)._buttons) {
//            auto &entity = createEntity();
//            entity.addComponent<rtype::components::Button>(button._text, button._texture, button._sprite, button._pos, button._rect);
//            entity.addComponent<rtype::components::Transform>(button._pos);
//            entity.addComponent<rtype::components::Clickable>(button._handler);
//        }

        for (auto &system : _menus.at(id)._systems) {
            this->registerSystem(system.first, system.second);
        }

        for (auto &handler : _menus.at(id)._handlers) {
            rtype::EntityFactory::createInputs(handler.first, std::move(handler.second));
        }
    }
} // namespace aecs
