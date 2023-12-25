//
// Created by qdesmettre on 04/12/23.
//

#ifndef EPITECH_R_TYPE_WORLD_HPP
#define EPITECH_R_TYPE_WORLD_HPP

#include "Entity.hpp"
#include "SystemBase.hpp"
#include "rtype/EntityFactory.hpp"
#include "rtype/components/ClientAdressComponent.hpp"
#include <SFML/Graphics.hpp>
#include <cstddef>
#include <map>
#include <memory>
#include <mutex>
#include <typeindex>
#include <vector>

namespace aecs
{
    class World
    {
      public:
        friend class Entity;
        World(bool isServer = false) :
            _isServer(isServer)
        {
        }
        ~World() = default;

        Entity &createEntity(size_t id = -1);

        template <typename T, typename... Args>
        T &addComponent(Entity &entity, Args &&...args)
        {
            return entity.addComponent<T>(std::forward<Args>(args)...);
        }

        template <typename T>
        void removeComponent(Entity &entity)
        {
            entity.removeComponent<T>();
        }

        void destroyEntity(Entity &entity);

        void update();
        void render();

        // TODO: put these 3 methods in GameState
        std::vector<std::byte> serialize(); // TODO
        void load(const std::vector<std::byte> &bytes)
        {
            load(bytes.data(), bytes.size());
        }
        void load(const void *data, std::size_t size); // TODO

        class GameState
        {
        }; // TODO: implement GameState
        void setTick(unsigned tick)
        {
            _tick = tick;
        }
        unsigned getTick() const
        {
            return _tick;
        }

        // Flush every gamestate before tick (including it)
        const GameState &getGameState(int tick) const; // TODO

        const ServerInputs getInputs(int tick = -1) const
        {
            if (tick == -1)
                tick = _tick;
            auto it = _renderInputs.find(tick);

            if (it == _renderInputs.end())
                return {};
            return it->second;
        }

        const ClientInputs getClientInputs(unsigned clientId, int tick = -1) const
        {
            if (tick == -1)
                tick = _tick;
            auto it = _renderInputs.find(tick);

            if (it == _renderInputs.end())
                return {};
            auto it2 = it->second.find(clientId);
            if (it2 == it->second.end())
                return {};
            return it2->second;
        }

        void setInputs(const ServerInputs &inputs)
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

        void setClientInputs(unsigned clientId, const ClientInputs &inputs)
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

        void setGameState(const GameState &gameState); // TODO

        EntityPtr getEntity(std::size_t id) const
        {
            auto it = _entities.find(id);

            if (it == _entities.end())
                return nullptr;
            return it->second;
        }

        std::vector<EntityPtr> getClients() const
        {
            std::vector<EntityPtr> clients;

            for (auto &[_, entity] : _entities) {
                if (entity->hasComponent<rtype::ClientAdressComponent>())
                    clients.push_back(entity);
            }
            return clients;
        }

        template <typename T, typename... Args>
        T &registerRenderSystem(Args &&...args)
        {
            static_assert(std::is_base_of_v<ISystem, T>, "T must inherit from ISystem");

            // To avoid changes in the entities vector while building the system
            const auto &constRefEntities = _entities;
            const auto &built = std::make_shared<T>(*this, constRefEntities, std::forward<Args>(args)...);
            _renderSystem = built;
            return *built;
        }

        template <typename T, typename... Args>
        T &registerSystem(int priority, Args &&...args)
        {
            static_assert(std::is_base_of_v<ISystem, T>, "T must inherit from ISystem");

            // To avoid changes in the entities vector while building the system
            const auto &constRefEntities = _entities;
            const auto &built = std::make_shared<T>(*this, constRefEntities, std::forward<Args>(args)...);
            _systems[typeid(T)] = {built, priority};
            sortSystems();
            return *built;
        }

        template <typename T>
        void setSystemPriority(int priority)
        {
            static_assert(std::is_base_of_v<ISystem, T>, "T must inherit from ISystem");

            _systems[typeid(T)].second = priority;
            sortSystems();
        }

        template <typename T>
        void removeSystem()
        {
            static_assert(std::is_base_of_v<ISystem, T>, "T must inherit from ISystem");

            _systems.erase(typeid(T));
            sortSystems();
        }

      private:
        void sortSystems();

        void onEntityAdded(const EntityPtr &entity);
        void onEntityRemoved(const EntityPtr &entity);
        void onEntityChanged(const EntityPtr &entity);
        void onEntityChanged(const Entity &entity);

        unsigned _tick = 0;
        std::map<std::size_t, EntityPtr> _entities;
        std::map<std::type_index, std::pair<std::shared_ptr<ISystem>, int>> _systems;
        std::vector<std::pair<ISystem *, int>> _sortedSystems;

        bool _isServer;
        std::shared_ptr<ISystem> _renderSystem;
        sf::Clock clock;
        std::map<unsigned, ServerInputs> _renderInputs;
        std::mutex _renderInputsMutex;
    };
} // namespace aecs

#endif // EPITECH_R_TYPE_WORLD_HPP
