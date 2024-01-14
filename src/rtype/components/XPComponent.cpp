//
// Created by qdesmettre on 14/01/24.
//

#include "rtype/components/XPComponent.hpp"
#include "shared/PacketBuilder.hpp"

std::vector<std::byte> rtype::XPComponent::encode() const
{
    PacketBuilder pb;
    pb << _xp << _xpForLevelup << _level;
    return pb.getData();
}

void rtype::XPComponent::decode(const std::vector<std::byte> &encoded)
{
    PacketBuilder pb;
    pb << encoded;
    pb >> _xp >> _xpForLevelup >> _level;
}
