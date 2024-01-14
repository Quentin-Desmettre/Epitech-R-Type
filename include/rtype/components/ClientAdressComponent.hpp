//
// Created by edo on 12/12/23.
//

#ifndef R_TYPE_CLIENTADRESSCOMPONENT_HPP
#define R_TYPE_CLIENTADRESSCOMPONENT_HPP
#include "aecs/AbstractComponent.hpp"

namespace rtype
{
    class ClientAdressComponent : public aecs::AbstractComponent
    {
      public:
        explicit ClientAdressComponent(unsigned int adress) :
            adress(adress){};
        ~ClientAdressComponent() override = default;
        const char *getName() const override
        {
            return "ClientAdressComponent";
        };
        unsigned int adress = 0;
    };
} // namespace rtype

#endif // R_TYPE_CLIENTADRESSCOMPONENT_HPP
