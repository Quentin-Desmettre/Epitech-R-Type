//
// Created by edo on 08/12/23.
//

#ifndef R_TYPE_PARALLAXCOMPONENT_HPP
#define R_TYPE_PARALLAXCOMPONENT_HPP

#include "aecs/AbstractComponent.hpp"
#include <SFML/Graphics.hpp>

namespace rtype
{
    class ParallaxComponent : public aecs::AbstractComponent
    {
      public:
        explicit ParallaxComponent(sf::Vector2f _speed) :
            _speed(_speed)
        {
            id = 9;
        };
        ~ParallaxComponent() override = default;

        sf::Vector2f _speed;
    };
} // namespace rtype

#endif // R_TYPE_PARALLAXCOMPONENT_HPP
