//
// Created by Clément Vandeville on 14/01/2024.
//

#ifndef PONG_TAGCOMPONENT_HPP
#define PONG_TAGCOMPONENT_HPP


#include "AbstractComponent.hpp"
#include <string>

class TagComponent : public aecs::AbstractComponent {
    public:
        explicit TagComponent(std::string tag);

        ~TagComponent() override = default;

        [[nodiscard]] std::vector<std::byte> encode() const override;

        void decode(const std::vector<std::byte> &encoded) override;

        [[nodiscard]] const char *getName() const override
        {
            return "TagComponent";
        };

        std::string tag;
};


#endif //PONG_TAGCOMPONENT_HPP
