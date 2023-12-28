//
// Created by qdesmettre on 05/12/23.
//

#ifndef R_TYPE_SPRITECOMPONENT_HPP
#define R_TYPE_SPRITECOMPONENT_HPP

#include "aecs/AbstractComponent.hpp"
#include <SFML/Graphics.hpp>

namespace rtype
{

    class SpriteComponent : public aecs::AbstractComponent
    {
      public:
        explicit SpriteComponent(const std::string &path, sf::Vector2f size = {0, 0},
                                 sf::IntRect TextureRect = {0, 0, 0, 0}, int zIndex = 0);
        ~SpriteComponent() override = default;

        sf::Sprite sprite;
        sf::Rect<int> rect;
        float time = 0;
        sf::Vector2f _size;
        int zIndex;

      private:
        sf::Texture _texture;
    };
} // namespace rtype

#endif // R_TYPE_SPRITECOMPONENT_HPP
