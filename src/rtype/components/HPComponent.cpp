//
// Created by edo on 23/12/23.
//

#include "rtype/components/HPComponent.hpp"
#include "shared/PacketBuilder.hpp"

std::vector<std::byte> rtype::HPComponent::encode() const
{
    PacketBuilder pb;
    pb << hp;
    return pb.getData();
}

void rtype::HPComponent::decode(const std::vector<std::byte> &encoded)
{
    PacketBuilder pb;
    pb << encoded;
    pb >> hp;
}