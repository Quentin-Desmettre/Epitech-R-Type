//
// Created by qdesmettre on 04/12/23.
//

#include "aecs/Entity.hpp"
#include "aecs/World.hpp"
#include "shared/PacketBuilder.hpp"

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
    std::vector<std::byte> Entity::encode() const
    {
        PacketBuilder pb;

        for (auto &component : _components)
        {
            pb << component.second->id;
            std::cout << component.second->id << std::endl;
            pb += component.second->encode();
        }
        return pb.getData();
    }

    void Entity::decode(const std::vector<std::byte> &encoded)
    {
        PacketBuilder pb;
        pb << encoded;

        while (pb)
        {
            int id;
            pb >> id;
            try {
                getComponentByComponentId(id).decode(pb.getSub());
            } catch (std::exception &e) {
                std::cerr << e.what() << std::endl;
            }
        }
    }

    AbstractComponent &Entity::getComponentByComponentId(int id)
    {
        for (auto &component : _components)
        {
            if (component.second->id == id)
                return *component.second;
        }
        throw std::runtime_error("Invalid component id");
    }
} // namespace aecs