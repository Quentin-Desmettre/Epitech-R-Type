//
// Created by edo on 23/12/23.
//

#include "PositionComponent.hpp"
#include "shared/PacketBuilder.hpp"

std::vector<std::byte> PositionComponent::encode() const
{
    PacketBuilder pb;
    pb << x << y;
    return pb.getData();
}

void PositionComponent::decode(const std::vector<std::byte> &encoded)
{
    PacketBuilder pb;
    pb << encoded;
    pb >> x >> y;
}
