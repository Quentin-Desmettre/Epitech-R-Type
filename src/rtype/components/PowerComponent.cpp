//
// Created by edo on 23/12/23.
//

#include "rtype/components/PowerComponent.hpp"
#include "shared/PacketBuilder.hpp"

rtype::PowerComponent::PowerType rtype::PowerComponent::getRandomPower()
{
    return static_cast<PowerType>(std::rand() % MAX_TYPE);
}

std::vector<std::byte> rtype::PowerComponent::encode() const
{
    PacketBuilder pb;
    pb << static_cast<std::uint8_t>(type);
    return pb.getData();
}

void rtype::PowerComponent::decode(const std::vector<std::byte> &encoded)
{
    PacketBuilder pb;
    std::uint8_t tmpType;
    pb << encoded;
    pb >> tmpType;
    type = static_cast<PowerType>(tmpType);
}