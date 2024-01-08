//
// Created by qdesmettre on 05/12/23.
//

#ifndef R_TYPE_TEXTCOMPONENT_HPP
#define R_TYPE_TEXTCOMPONENT_HPP

#include "aecs/AbstractComponent.hpp"
#include <SFML/Graphics.hpp>

namespace rtype
{
    class TextComponent : public aecs::AbstractComponent
    {
      public:
        explicit TextComponent(const std::string &text, int fontSize = 30, sf::Color color = sf::Color::White, int zIndex = 0, std::function<void()> &&onClick = nullptr);
        ~TextComponent() override = default;

        sf::Text _text;
        std::function<void()> onClick;
        int zIndex;

        const char *getName() const override {
            return "TextComponent";
        };

    };
} // namespace rtype

#endif // R_TYPE_TEXTCOMPONENT_HPP
