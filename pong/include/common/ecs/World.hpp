//
// Created by qdesmettre on 04/12/23.
//

#ifndef EPITECH_R_TYPE_WORLD_HPP
#define EPITECH_R_TYPE_WORLD_HPP

#include "Entity.hpp"
#include "SFML/Network/Packet.hpp"
#include "SystemBase.hpp"
#include "EntityFactory.hpp"
#include "ClientAddressComponent.hpp"
#include "shared/ArgParser.hpp"
#include "shared/Menu.hpp"
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
            explicit World(bool isServer = false, int ac = 0, char **av = nullptr);
            ~World() = default;

            // Methods
            Entity &createEntity(size_t id = -1);

            void addDecodeMap(const char *name, const std::function<void(aecs::Entity &, std::vector<std::byte>)> &map);

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

            [[nodiscard]] ServerInputs popInputs();

            [[nodiscard]] ServerInputs getInputs(unsigned tick = -1) const;

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
                auto sys = makeSystem<T>(priority, std::forward<Args>(args)...);
                _systems[typeid(T)] = {sys.system, sys.priority};
                sortSystems();
                return *std::dynamic_pointer_cast<T>(sys.system);
            }

            void registerSystem(const std::shared_ptr<ISystem> &system, int priority, std::type_index typeIndex);

            template <typename T, typename... Args>
            SystemData makeSystem(int priority, Args &&...args)
            {
                static_assert(std::is_base_of_v<ISystem, T>, "T must inherit from ISystem");

                // To avoid changes in the entities vector while building the system
                const auto &constRefEntities = _entities;
                std::shared_ptr<ISystem> built = std::make_shared<T>(*this, constRefEntities, std::forward<Args>(args)...);
                return {
                        .system = built,
                        .priority = priority,
                        .typeIndex = typeid(T),
                };
            }

            template <typename T>
            void setSystemPriority(int priority)
            {
                static_assert(std::is_base_of_v<ISystem, T>, "T must inherit from ISystem");

                _systems[typeid(T)].second = priority;
                sortSystems();
            }

            template <typename T>
            T &getSystem()
            {
                static_assert(std::is_base_of_v<ISystem, T>, "T must inherit from ISystem");

                if (_systems.find(typeid(T)) == _systems.end())
                    throw std::runtime_error("System not found");
                return *std::dynamic_pointer_cast<T>(_systems[typeid(T)].first);
            }

            template <typename T>
            void removeSystem()
            {
                static_assert(std::is_base_of_v<ISystem, T>, "T must inherit from ISystem");

                _systems.erase(typeid(T));
                sortSystems();
            }

            [[nodiscard]] bool getIsServer() const;

            [[nodiscard]] std::string getIp();
            [[nodiscard]] unsigned short getServerPort();
            [[nodiscard]] unsigned short getClientPort();
            [[nodiscard]] unsigned short getTcpPort();

            int addMenu(const Menu &menu, int id = -1);
            void goToMenu(int id);
            void leave();

            [[nodiscard]] std::size_t getEntityCount() const
            {
                return _entities.size();
            }

            unsigned playerConnected = 0;

        private:
            void sortSystems();
            void updateCurrentMenu();

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
            MouseInputs _mouseInputs;
            std::mutex _renderInputsMutex;
            ArgParser _argParser;
            std::map<int, Menu> _menus;
            bool _needLeave = false;
            int _currentMenu = -1;
            int _nextMenu = -1;
    };
} // namespace aecs

#endif // EPITECH_R_TYPE_WORLD_HPP
