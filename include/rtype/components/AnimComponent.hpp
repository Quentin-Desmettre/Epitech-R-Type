//
// Created by edo on 12/12/23.
//

#ifndef R_TYPE_ANIMCOMPONENT_HPP
#define R_TYPE_ANIMCOMPONENT_HPP

#include "aecs/AbstractComponent.hpp"

namespace rtype
{
    class AnimComponent : public aecs::AbstractComponent
    {
      public:
        AnimComponent(float time = 1) :
            timeToWait(time){
                id = 1;
            };
        ~AnimComponent() override = default;
        float timeSinceLastFrame = 0;
        float timeToWait = 1;
    };
} // namespace rtype

#endif // R_TYPE_ANIMCOMPONENT_HPP
