//
// Created by edo on 12/12/23.
//

#ifndef R_TYPE_CLIENTPORTCOMPONENT_HPP
#define R_TYPE_CLIENTPORTCOMPONENT_HPP
#include "aecs/AbstractComponent.hpp"

namespace rtype
{
    class ClientPortComponent : public aecs::AbstractComponent
    {
      public:
        explicit ClientPortComponent(unsigned short port) :
            port(port){id = 4;};
        ~ClientPortComponent() override = default;
        unsigned short port = 0;
    };
} // namespace rtype

#endif // R_TYPE_CLIENTPORTCOMPONENT_HPP
