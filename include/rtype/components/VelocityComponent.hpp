//
// Created by qdesmettre on 05/12/23.
//

#ifndef R_TYPE_VELOCITYCOMPONENT_HPP
#define R_TYPE_VELOCITYCOMPONENT_HPP

#include "aecs/AbstractComponent.hpp"

namespace rtype {
    class VelocityComponent: public aecs::AbstractComponent {
    public:
        VelocityComponent(float x, float y) : x(x), y(y) {}

        float x;
        float y;
    };
}

#endif //R_TYPE_VELOCITYCOMPONENT_HPP
