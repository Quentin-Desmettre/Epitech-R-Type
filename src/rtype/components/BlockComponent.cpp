//
// Created by qdesmettre on 06/01/24.
//

#include "rtype/components/BlockComponent.hpp"

#include <utility>

rtype::BlockComponent::BlockComponent(std::string texturePath, bool canBeShot, bool canBeHitBySmallBullet, float health,
                                      sf::IntRect rect, sf::Vector2f position, std::uint8_t usedSides) :
    canBeShot(canBeShot),
    canBeHitBySmallBullet(canBeHitBySmallBullet),
    health(health),
    texturePath(std::move(texturePath)),
    position(position),
    rect(rect),
    usedSides(usedSides)
{
}

std::vector<std::byte> rtype::BlockComponent::encode() const
{
    PacketBuilder pb;
    pb << canBeShot << canBeHitBySmallBullet << health << texturePath << position.x << position.y << rect.left << rect.top << rect.width << rect.height << usedSides;
    return pb.getData();
}

void rtype::BlockComponent::decode(const std::vector<std::byte> &encoded)
{
    PacketBuilder pb;
    pb << encoded;
    pb >> canBeShot >> canBeHitBySmallBullet >> health >> texturePath >> position.x >> position.y >> rect.left >> rect.top >> rect.width >> rect.height >> usedSides;
}

const char *rtype::BlockComponent::getName() const
{
    return "BlockComponent";
}
