//
// Created by qdesmettre on 05/12/23.
//

#ifndef R_TYPE_POSITIONCOMPONENT_HPP
#define R_TYPE_POSITIONCOMPONENT_HPP

#include "aecs/AbstractComponent.hpp"

namespace rtype {
    class PositionComponent: public aecs::AbstractComponent {
    public:
        PositionComponent(float x, float y) : x(x), y(y) {}

        float x;
        float y;
    };
}

#endif //R_TYPE_POSITIONCOMPONENT_HPP
