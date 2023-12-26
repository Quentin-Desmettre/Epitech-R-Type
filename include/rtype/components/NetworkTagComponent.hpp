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
        explicit NetworkTagComponent(bool active = true) :
            active(active){};
        ~NetworkTagComponent() override = default;
        bool active;
    };
} // namespace rtype
