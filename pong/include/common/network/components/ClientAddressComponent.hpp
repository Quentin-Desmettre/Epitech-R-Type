//
// Created by edo on 12/12/23.
//

#ifndef R_TYPE_CLIENTADRESSCOMPONENT_HPP
#define R_TYPE_CLIENTADRESSCOMPONENT_HPP

#include "AbstractComponent.hpp"

class ClientAddressComponent : public aecs::AbstractComponent {
    public:
        explicit ClientAddressComponent(unsigned int address);

        ~ClientAddressComponent() override = default;

        unsigned int address = 0;
};

#endif // R_TYPE_CLIENTADRESSCOMPONENT_HPP
