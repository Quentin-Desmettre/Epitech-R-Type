//
// Created by Clément Vandeville on 14/01/2024.
//

#ifndef PONG_BALLCOMPONENT_HPP
#define PONG_BALLCOMPONENT_HPP

#include "AbstractComponent.hpp"

class BallComponent : public aecs::AbstractComponent {
    public:
        explicit BallComponent() = default;

        ~BallComponent() override = default;

        [[nodiscard]] std::vector<std::byte> encode() const override;

        void decode(const std::vector<std::byte> &encoded) override;

        [[nodiscard]] const char *getName() const override
        {
            return "BallComponent";
        };
};
#endif //PONG_BALLCOMPONENT_HPP
