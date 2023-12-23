//
// Created by edo on 23/12/23.
//

#include "rtype/components/DamageCollisionComponent.hpp"
#include "shared/PacketBuilder.hpp"

std::vector<std::byte> rtype::DamageCollisionComponent::encode() const
{
    PacketBuilder pb;
    pb << team << damage << type << invulnerability;
    return pb.getData();
}

void rtype::DamageCollisionComponent::decode(const std::vector<std::byte> &encoded)
{
    PacketBuilder pb;
    pb << encoded;
    int tmp = 0;
    pb >> team >> damage >> tmp >> invulnerability;
    type = static_cast<ObjectType>(tmp);
}