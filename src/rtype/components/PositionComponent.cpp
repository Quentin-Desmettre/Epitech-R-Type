//
// Created by edo on 23/12/23.
//

#include "rtype/components/PositionComponent.hpp"
#include "shared/PacketBuilder.hpp"

std::vector<std::byte> rtype::PositionComponent::encode() const
{
    PacketBuilder pb;
    pb << x << y;
    return pb.getData();
}

void rtype::PositionComponent::decode(const std::vector<std::byte> &encoded)
{
    PacketBuilder pb;
    pb << encoded;
    pb >> x >> y;
}