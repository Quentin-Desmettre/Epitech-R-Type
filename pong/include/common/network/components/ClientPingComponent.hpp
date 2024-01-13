//
// Created by edo on 12/12/23.
//

#ifndef R_TYPE_CLIENTPINGCOMPONENT_HPP
#define R_TYPE_CLIENTPINGCOMPONENT_HPP

#include "AbstractComponent.hpp"
#include "SFML/Network.hpp"

class ClientPingComponent : public aecs::AbstractComponent {
    public:
        explicit ClientPingComponent() = default;

        ~ClientPingComponent() override = default;

        sf::Clock clock;
};

#endif // R_TYPE_CLIENTPINGCOMPONENT_HPP
