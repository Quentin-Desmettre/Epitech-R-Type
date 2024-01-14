//
// Created by Clément Vandeville on 14/01/2024.
//

#include "BallComponent.hpp"
#include "shared/PacketBuilder.hpp"

std::vector<std::byte> BallComponent::encode() const
{
    PacketBuilder pb;
    return pb.getData();
}

void BallComponent::decode(const std::vector<std::byte> &encoded)
{
    PacketBuilder pb;
    pb << encoded;
}
