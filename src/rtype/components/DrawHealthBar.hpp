//
// Created by qdesmettre on 13/01/24.
//

#ifndef R_TYPE_DRAWHEALTHBAR_HPP
#define R_TYPE_DRAWHEALTHBAR_HPP

#include "aecs/AbstractComponent.hpp"

namespace rtype {
    class DrawHealthBar: public aecs::AbstractComponent {
    public:
        DrawHealthBar() = default;
        ~DrawHealthBar() override = default;

        const char *getName() const override {
            return "DrawHealthBar";
        };
    };
}


#endif //R_TYPE_DRAWHEALTHBAR_HPP
