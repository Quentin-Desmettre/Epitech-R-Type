//
// Created by qdesmettre on 04/12/23.
//

#ifndef EPITECH_R_TYPE_WORLD_HPP
#define EPITECH_R_TYPE_WORLD_HPP

#include "rtype/EntityFactory.hpp"
#include "Entity.hpp"
#include "SystemBase.hpp"
#include <cstddef>
#include <map>
#include <memory>
#include <mutex>
#include <typeindex>
#include <SFML/Graphics.hpp>
#include <vector>

namespace aecs
{

    class World
    {
      public:
        friend class Entity;
        World() = default;
        ~World() = default;

        Entity &createEntity();

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

        std::map<std::size_t, EntityPtr> _entities;
        std::map<std::type_index, std::pair<std::shared_ptr<ISystem>, int>> _systems;
        std::vector<std::pair<ISystem *, int>> _sortedSystems;

        std::shared_ptr<ISystem> _renderSystem;
        sf::Clock clock;
        std::vector<RenderInput> _renderInputs;
        std::mutex _renderInputsMutex;
    };
} // namespace aecs

#endif // EPITECH_R_TYPE_WORLD_HPP
