//
// Created by edo on 06/12/23.
//

#ifndef R_TYPE_MYPLAYERCOMPONENT_HPP
#define R_TYPE_MYPLAYERCOMPONENT_HPP

#include "aecs/AbstractComponent.hpp"

namespace rtype
{
    class MyPlayerComponent : public aecs::AbstractComponent
    {
      public:
        MyPlayerComponent() = default;
        ~MyPlayerComponent() override = default;
        float timeSinceLastShoot = 0;
        float timeInShift = 0;
    };
} // namespace rtype


#endif // R_TYPE_MYPLAYERCOMPONENT_HPP
