//
// Created by qdesmettre on 05/12/23.
//

#ifndef R_TYPE_POSITIONCOMPONENT_HPP
#define R_TYPE_POSITIONCOMPONENT_HPP

#include "aecs/AbstractComponent.hpp"

namespace rtype
{
    class PositionComponent : public aecs::AbstractComponent
    {
      public:
        PositionComponent(float x, float y) :
            x(x),
            y(y)
        {
            id = 11;
        }
        ~PositionComponent() override = default;
        [[nodiscard]] std::vector<std::byte> encode() const override;
        void decode(const std::vector<std::byte> &encoded) override;

        float x;
        float y;
    };
} // namespace rtype

#endif // R_TYPE_POSITIONCOMPONENT_HPP
