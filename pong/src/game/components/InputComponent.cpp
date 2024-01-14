//
// Created by qdesmettre on 05/12/23.
//

#include "InputComponent.hpp"

aecs::InputComponent::InputComponent(int input, std::function<void()> &&onInput) :
        input(input),
        onInput(std::move(onInput)){};
