//
// Created by qdesmettre on 04/12/23.
//

#include "aecs/Entity.hpp"
#include "aecs/World.hpp"
#include "shared/PacketBuilder.hpp"

namespace aecs
{
    std::size_t Entity::_idCounter = 0;
    Entity::Entity(World &world, std::size_t id) :
        _world(world)
    {
        if (id == (std::size_t)(-1))
            _id = _idCounter++;
        else
            _id = id;
    }

    constexpr unsigned int Entity::hashString(const char *str, int h)
    {
        return !str[h] ? 5381 : (hashString(str, h + 1) * 33) ^ str[h];
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
        return std::all_of(components.begin(), components.end(), [this](const auto &component) {
            return _components.find(component) != _components.end();
        });
    }
    std::vector<std::byte> Entity::encode() const
    {
        PacketBuilder pb;

        for (auto &component : _components) {
            pb << hashString(typeid(*component.second).name());
            pb += component.second->encode();
        }
        std::vector<std::byte> data = pb.getData();
        pb.clear();
        pb << static_cast<unsigned>(getId());
        pb += data;
        return pb.getData();
    }

    void Entity::decode(const std::vector<std::byte> &encoded)
    {
        PacketBuilder pb;
        pb << encoded;
        pb.pass(sizeof(int) + sizeof(ushort));

        while (pb) {
            uint id;
            pb >> id;
            std::vector<std::byte> sub = pb.getSub();
            try {
                getComponentByComponentId(id).decode(sub);
            } catch (std::exception &e) {
                std::cerr << e.what() << std::endl;
            }
        }
    }

    AbstractComponent &Entity::getComponentByComponentId(uint id)
    {
        for (auto &component : _components) {
            if (hashString(typeid(*component.second).name()) == id)
                return *component.second;
        }
        throw std::runtime_error("Invalid component id");
    }
} // namespace aecs
