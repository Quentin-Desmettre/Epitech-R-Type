//
// Created by edo on 23/12/23.
//

#include "rtype/components/PowerComponent.hpp"
#include "shared/PacketBuilder.hpp"

std::vector<std::byte> rtype::PowerComponent::encode() const
{
    PacketBuilder pb;
    pb << isPowerUp;
    return pb.getData();
}

void rtype::PowerComponent::decode(const std::vector<std::byte> &encoded)
{
    PacketBuilder pb;
    pb << encoded;
    pb >> isPowerUp;
}