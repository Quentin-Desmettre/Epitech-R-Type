//
// Created by qdesmettre on 06/01/24.
//

#ifndef R_TYPE_BLOCKCOMPONENT_HPP
#define R_TYPE_BLOCKCOMPONENT_HPP

#include "aecs/AbstractComponent.hpp"
#include "shared/PacketBuilder.hpp"

namespace rtype
{
    class BlockComponent : public aecs::AbstractComponent
    {
      public:
        explicit BlockComponent(std::string texturePath = "", bool canBeShot = false,
                                bool canBeHitBySmallBullet = false, float health = 0, sf::Vector2f position = {0, 0});
        ~BlockComponent() override = default;

        const char *getName() const override;
        [[nodiscard]] std::vector<std::byte> encode() const override;
        void decode(const std::vector<std::byte> &encoded) override;

        bool canBeShot;
        bool canBeHitBySmallBullet;
        float health;
        std::string texturePath;
        sf::Vector2f position;
    };
} // namespace rtype

#endif // R_TYPE_BLOCKCOMPONENT_HPP
