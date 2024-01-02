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
        explicit AnimComponent(float time = 1) :
            timeToWait(time){};
        ~AnimComponent() override = default;
        const char *getName() const override {
            return "AnimComponent";
        };
        float timeSinceLastFrame = 0;
        float timeToWait = 1;
    };
} // namespace rtype

#endif // R_TYPE_ANIMCOMPONENT_HPP
