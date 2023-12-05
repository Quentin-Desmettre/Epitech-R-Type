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

} // namespace aecs