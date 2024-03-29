//
// Created by qdesmettre on 05/12/23.
//

#ifndef R_TYPE_VELOCITYCOMPONENT_HPP
#define R_TYPE_VELOCITYCOMPONENT_HPP

#include "aecs/AbstractComponent.hpp"

namespace rtype
{
    class VelocityComponent : public aecs::AbstractComponent
    {
      public:
        explicit VelocityComponent(float x = 0, float y = 0) :
            x(x),
            y(y)
        {
        }
        ~VelocityComponent() override = default;
        [[nodiscard]] std::vector<std::byte> encode() const override;
        void decode(const std::vector<std::byte> &encoded) override;

        float x;
        float y;

        const char *getName() const override
        {
            return "VelocityComponent";
        };
    };
} // namespace rtype

#endif // R_TYPE_VELOCITYCOMPONENT_HPP
