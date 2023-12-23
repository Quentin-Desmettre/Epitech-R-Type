//
// Created by edo on 23/12/23.
//

#include "rtype/components/PlayerComponent.hpp"
#include "shared/PacketBuilder.hpp"

std::string rtype::PlayerComponent::encode() const
{
    PacketBuilder pb;
    pb << timeInShift << timeSinceLastShoot;
    return pb.getData();
}

void rtype::PlayerComponent::decode(const std::string &encoded)
{
    PacketBuilder pb;
    pb << encoded;
    pb >> timeInShift >> timeSinceLastShoot;
}