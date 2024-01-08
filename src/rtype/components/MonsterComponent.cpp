//
// Created by edo on 23/12/23.
//

#include "rtype/components/MonsterComponent.hpp"
#include "shared/PacketBuilder.hpp"

std::vector<std::byte> rtype::MonsterComponent::encode() const
{
    PacketBuilder pb;
    pb << _lil << timeSinceLastShoot << _isShooting;
    return pb.getData();
}

void rtype::MonsterComponent::decode(const std::vector<std::byte> &encoded)
{
    PacketBuilder pb;
    pb << encoded;
    pb >> _lil >> timeSinceLastShoot >> _isShooting;
}