//
// Created by edo on 23/12/23.
//

#include "rtype/components/PlayerComponent.hpp"
#include "shared/PacketBuilder.hpp"
#include <cstdint>

std::uint8_t rtype::PlayerComponent::_maxPlayerId = 1;

rtype::PlayerComponent::PlayerComponent() :
    timeInShift(0),
    timeSinceLastShoot(0),
    playerId(_maxPlayerId++)
{
}

std::vector<std::byte> rtype::PlayerComponent::encode() const
{
    PacketBuilder pb;
    pb << timeInShift << timeSinceLastShoot << playerId;
    return pb.getData();
}

void rtype::PlayerComponent::decode(const std::vector<std::byte> &encoded)
{
    PacketBuilder pb;
    int tmp;

    pb << encoded;
    pb >> timeInShift >> timeSinceLastShoot >> tmp;
    playerId = tmp;
}
