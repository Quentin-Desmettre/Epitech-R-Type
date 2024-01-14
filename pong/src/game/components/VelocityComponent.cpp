//
// Created by edo on 23/12/23.
//

#include "VelocityComponent.hpp"
#include "shared/PacketBuilder.hpp"

std::vector<std::byte> VelocityComponent::encode() const
{
    PacketBuilder pb;
    pb << x << y;
    return pb.getData();
}

void VelocityComponent::decode(const std::vector<std::byte> &encoded)
{
    PacketBuilder pb;
    pb << encoded;
    pb >> x >> y;
}
