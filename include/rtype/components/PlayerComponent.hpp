//
// Created by edo on 06/12/23.
//

#ifndef R_TYPE_PLAYERCOMPONENT_HPP
#define R_TYPE_PLAYERCOMPONENT_HPP

#include "aecs/AbstractComponent.hpp"

namespace rtype
{
    class PlayerComponent : public aecs::AbstractComponent
    {
      public:
        PlayerComponent() = default;
        ~PlayerComponent() override = default;
    };
} // namespace rtype


#endif // R_TYPE_PLAYERCOMPONENT_HPP
