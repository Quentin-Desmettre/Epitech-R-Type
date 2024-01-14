//
// Created by edo on 28/12/23.
//

#include "rtype/components/BulletComponent.hpp"
#include "shared/PacketBuilder.hpp"

std::vector<std::byte> rtype::BulletComponent::encode() const
{
    PacketBuilder pb;
    pb << big << shotBy;
    return pb.getData();
}

void rtype::BulletComponent::decode(const std::vector<std::byte> &encoded)
{
    PacketBuilder pb;
    pb << encoded;
    pb >> big >> shotBy;
}