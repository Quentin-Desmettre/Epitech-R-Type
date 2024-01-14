//
// Created by edo on 03/01/24.
//
#include "rtype/components/NodeComponent.hpp"
#include "shared/PacketBuilder.hpp"

std::vector<std::byte> rtype::NodeComponent::encode() const
{
    PacketBuilder pb;
    pb << delay << rotate;
    return pb.getData();
}

void rtype::NodeComponent::decode(const std::vector<std::byte> &encoded)
{
    PacketBuilder pb;
    pb << encoded;
    pb >> delay >> rotate;
}