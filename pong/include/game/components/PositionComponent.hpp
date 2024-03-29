//
// Created by qdesmettre on 05/12/23.
//

#ifndef R_TYPE_POSITIONCOMPONENT_HPP
#define R_TYPE_POSITIONCOMPONENT_HPP

#include "AbstractComponent.hpp"

class PositionComponent : public aecs::AbstractComponent {
    public:
        explicit PositionComponent(float x = 0, float y = 0) :
                x(x),
                y(y)
        {
        }

        ~PositionComponent() override = default;

        [[nodiscard]] std::vector<std::byte> encode() const override;

        void decode(const std::vector<std::byte> &encoded) override;

        [[nodiscard]] const char *getName() const override
        {
            return "PositionComponent";
        };

        float x;
        float y;
};

#endif // R_TYPE_POSITIONCOMPONENT_HPP
