//
// Created by edo on 23/12/23.
//

#include "rtype/components/MonsterComponent.hpp"
#include "shared/PacketBuilder.hpp"

std::string rtype::MonsterComponent::encode() const
{
    PacketBuilder pb;
    pb << _lil << timeSinceLastShoot;
    return pb.getData();
}

void rtype::MonsterComponent::decode(const std::string &encoded)
{
    PacketBuilder pb;
    pb << encoded;
    pb >> _lil >> timeSinceLastShoot;
}