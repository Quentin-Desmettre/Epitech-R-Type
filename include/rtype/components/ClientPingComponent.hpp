//
// Created by edo on 12/12/23.
//

#ifndef R_TYPE_CLIENTPINGCOMPONENT_HPP
#define R_TYPE_CLIENTPINGCOMPONENT_HPP
#include "aecs/AbstractComponent.hpp"
#include <SFML/Network.hpp>

namespace rtype
{
    class ClientPingComponent : public aecs::AbstractComponent
    {
      public:
        explicit ClientPingComponent() = default;
        ~ClientPingComponent() override = default;

        const char *getName() const override
        {
            return "ClientPingComponent";
        };
        sf::Clock clock;
    };
} // namespace rtype

#endif // R_TYPE_CLIENTPINGCOMPONENT_HPP
