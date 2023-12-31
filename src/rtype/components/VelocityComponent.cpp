//
// Created by edo on 23/12/23.
//

#include "rtype/components/VelocityComponent.hpp"
#include "shared/PacketBuilder.hpp"

std::vector<std::byte> rtype::VelocityComponent::encode() const
{
    PacketBuilder pb;
    pb << x << y;
    return pb.getData();
}

void rtype::VelocityComponent::decode(const std::vector<std::byte> &encoded)
{
    PacketBuilder pb;
    pb << encoded;
    pb >> x >> y;
}