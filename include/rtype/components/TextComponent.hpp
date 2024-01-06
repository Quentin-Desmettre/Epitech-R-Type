//
// Created by qdesmettre on 05/12/23.
//

#ifndef R_TYPE_SPRITECOMPONENT_HPP
#define R_TYPE_SPRITECOMPONENT_HPP

#include "aecs/AbstractComponent.hpp"
#include <SFML/Graphics.hpp>

namespace rtype
{

    class TextComponent : public aecs::AbstractComponent
    {
      public:
        explicit TextComponent(const std::string &text, int fontSize = 30, sf::Vector2f pos = {0, 0}, sf::Color color = sf::Color::White, int zIndex = 0);
        ~TextComponent() override = default;

        sf::Text _text;
        int zIndex;

        const char *getName() const override {
            return "TextComponent";
        };

      private:
        sf::Font _font;
    };
} // namespace rtype

#endif // R_TYPE_SPRITECOMPONENT_HPP
