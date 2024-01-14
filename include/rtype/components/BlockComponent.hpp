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
        enum UsedSide : std::uint8_t {
            NONE = 0,
            LEFT = 1,
            RIGHT = 2,
            TOP = 4,
            BOTTOM = 8,
            TOP_LEFT = 16,
            TOP_RIGHT = 32,
            BOTTOM_LEFT = 64,
            BOTTOM_RIGHT = 128
        };
        std::uint8_t operator|(UsedSide side) const
        {
            return usedSides | side;
        }

        explicit BlockComponent(std::string texturePath = "", bool canBeShot = false,
                                bool canBeHitBySmallBullet = false, float health = 0,
                                sf::IntRect textureRect = {0, 0, 0, 0}, sf::Vector2f position = {0, 0},
                                std::uint8_t usedSides = NONE);
        ~BlockComponent() override = default;

        const char *getName() const override;
        [[nodiscard]] std::vector<std::byte> encode() const override;
        void decode(const std::vector<std::byte> &encoded) override;

        bool canBeShot;
        bool canBeHitBySmallBullet;
        float health;
        std::string texturePath;
        sf::Vector2f position;
        sf::IntRect rect;
        std::uint8_t usedSides;
    };
} // namespace rtype

#endif // R_TYPE_BLOCKCOMPONENT_HPP
