//
// Created by qdesmettre on 05/12/23.
//

#ifndef R_TYPE_INPUTCOMPONENT_HPP
#define R_TYPE_INPUTCOMPONENT_HPP

#include "SFML/Graphics.hpp"
#include "aecs/AbstractComponent.hpp"
#include <functional>

namespace aecs
{
    class InputComponent : public aecs::AbstractComponent
    {
      public:
        explicit InputComponent(int input, std::function<void()> &&onInput);
        ~InputComponent() override = default;

        int input;
        std::function<void()> &&onInput;

        [[nodiscard]] const char *getName() const override
        {
            return "InputComponent";
        }
    };
} // namespace aecs

#endif // R_TYPE_INPUTCOMPONENT_HPP
