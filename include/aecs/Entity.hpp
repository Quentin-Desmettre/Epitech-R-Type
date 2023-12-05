//
// Created by qdesmettre on 04/12/23.
//

#ifndef R_TYPE_ENTITY_HPP
#define R_TYPE_ENTITY_HPP

#include "AbstractComponent.hpp"
#include <cstddef>
#include <map>
#include <memory>
#include <typeindex>
#include <vector>
#include <algorithm>

namespace aecs
{

class World;

class Entity
{

  public:
    explicit Entity(World &world);
    Entity() = delete;
    Entity(const Entity &) = delete;
    Entity(Entity &&) = delete;
    ~Entity() = default;

    [[nodiscard]] std::size_t getId() const;

    template <typename T, typename... Args>
    T &addComponent(Args &&...args)
    {
        static_assert(std::is_base_of<AbstractComponent, T>::value, "T must inherit from AbstractComponent");

        auto component = std::make_shared<T>(std::forward<Args>(args)...);
        _components.emplace(typeid(T), component);
        notifyWorldEntityChanged();
        return *component;
    }

    template <typename T>
    void removeComponent()
    {
        static_assert(std::is_base_of<AbstractComponent, T>::value, "T must inherit from AbstractComponent");

        if (!hasComponent<T>())
            return;
        notifyWorldEntityChanged();
        _components.erase(typeid(T));
    }

    template <typename T>
    [[nodiscard]] bool hasComponent() const
    {
        static_assert(std::is_base_of<AbstractComponent, T>::value, "T must inherit from AbstractComponent");

        return _components.find(typeid(T)) != _components.end();
    }

    [[nodiscard]] bool hasComponents(const std::vector<std::type_index> &components) const;

    template <typename T>
    [[nodiscard]] const T &getComponent() const
    {
        static_assert(std::is_base_of<AbstractComponent, T>::value, "T must inherit from AbstractComponent");

        return _components.at(typeid(T));
    }

    template <typename T>
    [[nodiscard]] T &getComponent()
    {
        static_assert(std::is_base_of<AbstractComponent, T>::value, "T must inherit from AbstractComponent");

        auto *component = dynamic_cast<T *>(_components.at(typeid(T)).get());
        if (!component)
            throw std::runtime_error("Invalid component type");
        return *component;
    }

  private:
    void notifyWorldEntityChanged();

    std::size_t _id;
    static std::size_t _idCounter;
    World &_world;
    std::map<std::type_index, std::shared_ptr<AbstractComponent>> _components;
};

typedef std::shared_ptr<Entity> EntityPtr;

} // namespace aecs

#endif // R_TYPE_ENTITY_HPP
