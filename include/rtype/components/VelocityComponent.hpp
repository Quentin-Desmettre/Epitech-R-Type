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
        VelocityComponent(float x, float y) :
            x(x),
            y(y)
        {
        }
        ~VelocityComponent() override = default;
        [[nodiscard]] std::vector<std::byte> encode() const override;
        void decode(const std::vector<std::byte> &encoded) override;

        float x;
        float y;
    };
} // namespace rtype

#endif // R_TYPE_VELOCITYCOMPONENT_HPP
