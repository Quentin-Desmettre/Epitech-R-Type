//
// Created by qdesmettre on 06/01/24.
//

#include "rtype/components/BlockComponent.hpp"

#include <utility>

rtype::BlockComponent::BlockComponent(std::string texturePath, bool canBeShot, bool canBeHitBySmallBullet, float health,
                                      sf::Vector2f position) :
    canBeShot(canBeShot),
    canBeHitBySmallBullet(canBeHitBySmallBullet),
    health(health),
    texturePath(std::move(texturePath)),
    position(position)
{
}

std::vector<std::byte> rtype::BlockComponent::encode() const
{
    PacketBuilder pb;
    pb << canBeShot << canBeHitBySmallBullet << health << texturePath << position.x << position.y;
    return pb.getData();
}

void rtype::BlockComponent::decode(const std::vector<std::byte> &encoded)
{
    PacketBuilder pb;
    pb << encoded;
    pb >> canBeShot >> canBeHitBySmallBullet >> health >> texturePath >> position.x >> position.y;
}

const char *rtype::BlockComponent::getName() const
{
    return "BlockComponent";
}
