//
// Created by edo on 19/12/23.
//

#ifndef R_TYPE_POWERCOMPONENT_HPP
#define R_TYPE_POWERCOMPONENT_HPP
#include "aecs/AbstractComponent.hpp"

namespace rtype
{
    class PowerComponent : public aecs::AbstractComponent
    {
      public:
        enum Type {
            SHOOT,
            NONE,
        };

        explicit PowerComponent(bool isPowerUp = true, Type type = Type::SHOOT) :
            isPowerUp(isPowerUp),
            type(type){};
        ~PowerComponent() override = default;
        bool isPowerUp = true;
        Type type = Type::SHOOT;

        const char *getName() const override
        {
            return "PowerComponent";
        };
        [[nodiscard]] std::vector<std::byte> encode() const override;
        void decode(const std::vector<std::byte> &encoded) override;
    };
} // namespace rtype

#endif // R_TYPE_POWERCOMPONENT_HPP
