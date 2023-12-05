//
// Created by qdesmettre on 04/12/23.
//

#include "aecs/Entity.hpp"
#include "aecs/World.hpp"

namespace aecs
{
    std::size_t Entity::_idCounter = 0;
    Entity::Entity(World &world) :
        _id(_idCounter++),
        _world(world)
    {
    }

    std::size_t Entity::getId() const
    {
        return _id;
    }

    void Entity::notifyWorldEntityChanged()
    {
        _world.onEntityChanged(*this);
    }

    bool Entity::hasComponents(const std::vector<std::type_index> &components) const
    {
        return std::ranges::all_of(components, [this](const auto &component) {
            return _components.find(component) != _components.end();
        });
    }
} // namespace aecs