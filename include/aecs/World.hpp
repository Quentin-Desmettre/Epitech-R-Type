//
// Created by qdesmettre on 04/12/23.
//

#ifndef EPITECH_R_TYPE_WORLD_HPP
#define EPITECH_R_TYPE_WORLD_HPP

#include "Entity.hpp"
#include "SFML/Network/Packet.hpp"
#include "SystemBase.hpp"
#include "rtype/EntityFactory.hpp"
#include "rtype/components/ClientAdressComponent.hpp"
#include "shared/PacketBuilder.hpp"
#include <SFML/Graphics.hpp>
#include <cstddef>
#include <functional>
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
        // Typedefs & structs
        typedef std::map<unsigned, std::vector<std::byte>> EncodedEntities;
        struct EncodedGameState {
            EncodedEntities encodedEntities;
            unsigned tick;
        };

        // Ctor / dtor
        friend class Entity;
        explicit World(bool isServer = false);
        ~World() = default;

        // Methods
        Entity &createEntity(size_t id = -1);

        void addDecodeMap(const std::type_info &type,
                          const std::function<void(aecs::Entity &, std::vector<std::byte>)> &map);

        void decodeNewEntity(Entity &entity, const std::vector<std::byte> &data);

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

        [[nodiscard]] std::vector<std::byte> serialize() const;

        void load(const EncodedGameState &entities);

        void setTick(unsigned tick);
        [[nodiscard]] unsigned getTick() const;

        [[nodiscard]] ServerInputs getInputs(unsigned tick = -1) const;

        [[nodiscard]] ClientInputs getClientInputs(unsigned clientId, std::size_t tick = -1) const;

        void setInputs(const ServerInputs &inputs);

        void setClientInputs(unsigned clientId, const ClientInputs &inputs);

        [[nodiscard]] EntityPtr getEntity(std::size_t id) const;

        void setClientId(unsigned clientId);
        [[nodiscard]] unsigned getClientId() const;

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

        [[nodiscard]] bool getIsServer() const;

      private:
        void sortSystems();

        void onEntityAdded(const EntityPtr &entity);
        void onEntityRemoved(const EntityPtr &entity);
        void onEntityChanged(const EntityPtr &entity);
        void onEntityChanged(const Entity &entity);

        unsigned _tick = 0;
        std::map<std::size_t, EntityPtr> _entities;
        unsigned _clientId = 0;
        std::map<std::type_index, std::pair<std::shared_ptr<ISystem>, int>> _systems;
        std::vector<std::pair<ISystem *, int>> _sortedSystems;
        std::map<uint, std::function<void(aecs::Entity &, std::vector<std::byte>)>> decodeMap;

        bool _isServer;
        std::shared_ptr<ISystem> _renderSystem;
        sf::Clock clock;
        std::map<unsigned, ServerInputs> _renderInputs;
        std::mutex _renderInputsMutex;
    };
} // namespace aecs

#endif // EPITECH_R_TYPE_WORLD_HPP
