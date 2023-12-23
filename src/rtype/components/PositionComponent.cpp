//
// Created by edo on 23/12/23.
//

#include "rtype/components/PositionComponent.hpp"
#include "shared/PacketBuilder.hpp"

std::string rtype::PositionComponent::encode() const
{
    PacketBuilder pb;
    pb << x << y;
    return pb.getData();
}

void rtype::PositionComponent::decode(const std::string &encoded)
{
    PacketBuilder pb;
    pb << encoded;
    pb >> x >> y;
}