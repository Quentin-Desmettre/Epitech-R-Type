//
// Created by qdesmettre on 13/01/24.
//

#include "rtype/components/DifficultyComponent.hpp"
#include "shared/PacketBuilder.hpp"

std::vector<std::byte> rtype::DifficultyComponent::encode() const
{
    PacketBuilder pb;
    pb << difficulty;
    return pb.getData();
}

void rtype::DifficultyComponent::decode(const std::vector<std::byte> &encoded)
{
    PacketBuilder pb;
    pb << encoded;
    pb >> difficulty;
}
