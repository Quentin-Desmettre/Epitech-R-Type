//
// Created by edo on 09/01/24.
//

#ifndef R_TYPE_BULLETGENCOMPONENT_HPP
#define R_TYPE_BULLETGENCOMPONENT_HPP

#include "aecs/AbstractComponent.hpp"
#include <SFML/Graphics.hpp>

namespace rtype
{
    class BulletGenComponent : public aecs::AbstractComponent
    {
      public:
        explicit BulletGenComponent(float timeBetween, bool big = false, bool shoot = true, int nb = 1, float padding = 0, int team = 1, sf::Vector2f velocity = {-50, 0}) :
            shoot(shoot), timeBetween(timeBetween), big(big), team(team), velocity(velocity), nb(nb), padding(padding){};
        ~BulletGenComponent() override = default;
        const char *getName() const override
        {
            return "BulletGenComponent";
        };
        bool shoot = true;
        float timeBetween;
        float time = 0;
        bool big;
        int team;
        sf::Vector2f velocity;
        int nb;
        float padding;
    };
} // namespace rtype


#endif // R_TYPE_BULLETGENCOMPONENT_HPP
