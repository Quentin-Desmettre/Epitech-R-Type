//
// Created by edo on 23/12/23.
//

#include "rtype/components/PlayerComponent.hpp"
#include "shared/PacketBuilder.hpp"

std::vector<std::byte> rtype::PlayerComponent::encode() const
{
    PacketBuilder pb;
    pb << timeInShift << timeSinceLastShoot << clientID;
    return pb.getData();
}

void rtype::PlayerComponent::decode(const std::vector<std::byte> &encoded)
{
    PacketBuilder pb;
    pb << encoded;
    pb >> timeInShift >> timeSinceLastShoot >> clientID;
}