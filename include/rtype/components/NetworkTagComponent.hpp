/*
** EPITECH PROJECT, 2023
** Epitech-R-Type
** File description:
** NetworkTagComponent
*/

#pragma once

#include "aecs/AbstractComponent.hpp"

namespace rtype
{
    class NetworkTagComponent : public aecs::AbstractComponent
    {
      public:
        explicit NetworkTagComponent(bool active = true)
        {
            id = 16;
        };
        ~NetworkTagComponent() override = default;
        bool active;
    };
} // namespace rtype
