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
        enum PowerType {
            DOUBLE_SHOT,
            INVERSE_DIR,
            HEALTH_PACK,
            MAX_TYPE // Keep last
        };
        static PowerType getRandomPower();

        explicit PowerComponent(PowerType type = DOUBLE_SHOT) :
            type(type){};

        ~PowerComponent() override = default;
        PowerType type;

        const char *getName() const override
        {
            return "PowerComponent";
        };
        [[nodiscard]] std::vector<std::byte> encode() const override;
        void decode(const std::vector<std::byte> &encoded) override;
    };
} // namespace rtype

#endif // R_TYPE_POWERCOMPONENT_HPP
