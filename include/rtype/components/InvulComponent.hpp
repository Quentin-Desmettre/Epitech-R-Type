//
// Created by qdesmettre on 13/01/24.
//

#ifndef R_TYPE_INVULCOMPONENT_HPP
#define R_TYPE_INVULCOMPONENT_HPP

#include "aecs/AbstractComponent.hpp"

namespace rtype {

    class InvulComponent: public aecs::AbstractComponent {
    public:
        InvulComponent() = default;
        ~InvulComponent() override = default;

        const char *getName() const override {
            return "InvulComponent";
        }
    };
}

#endif //R_TYPE_INVULCOMPONENT_HPP
