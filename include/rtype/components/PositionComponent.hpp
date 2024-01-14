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
        explicit PositionComponent(float x = 0, float y = 0, bool deleteIfOutOfScreen = false) :
            x(x),
            y(y),
            deleteIfOutOfScreen(deleteIfOutOfScreen)
        {
        }
        ~PositionComponent() override = default;
        [[nodiscard]] std::vector<std::byte> encode() const override;
        void decode(const std::vector<std::byte> &encoded) override;

        float x;
        float y;
        bool deleteIfOutOfScreen;

        [[nodiscard]] const char *getName() const override
        {
            return "PositionComponent";
        };
    };
} // namespace rtype

#endif // R_TYPE_POSITIONCOMPONENT_HPP
