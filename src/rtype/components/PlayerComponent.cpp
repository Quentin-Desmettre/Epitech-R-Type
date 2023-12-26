//
// Created by edo on 23/12/23.
//

#include "rtype/components/PlayerComponent.hpp"
#include "shared/PacketBuilder.hpp"

std::uint8_t rtype::PlayerComponent::_maxPlayerId = 1;

rtype::PlayerComponent::PlayerComponent() :
    timeInShift(0),
    timeSinceLastShoot(0),
    playerId(_maxPlayerId++)
{
    id = 10;
}

std::vector<std::byte> rtype::PlayerComponent::encode() const
{
    PacketBuilder pb;
    pb << timeInShift << timeSinceLastShoot;
    return pb.getData();
}

void rtype::PlayerComponent::decode(const std::vector<std::byte> &encoded)
{
    PacketBuilder pb;
    pb << encoded;
    pb >> timeInShift >> timeSinceLastShoot;
}