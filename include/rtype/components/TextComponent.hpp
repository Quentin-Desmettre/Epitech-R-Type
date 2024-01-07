//
// Created by Clément Vandeville on 07/01/2024.
//

#ifndef R_TYPE_TEXTCOMPONENT_HPP
#define R_TYPE_TEXTCOMPONENT_HPP

#include "aecs/AbstractComponent.hpp"
#include <SFML/Graphics.hpp>

namespace rtype {

    class TextComponent : public aecs::AbstractComponent {
        public:
            explicit TextComponent(std::string string, int fontSize = 12, sf::Color color = sf::Color::White, int zIndex = 0);

            ~TextComponent() override = default;

            int zIndex;
            sf::Text text;

            static sf::Font font;
            static bool loadFont;

        private:
            std::string _string;
            int _fontSize;
            sf::Color _color;

    };
} // namespace rtype


#endif //R_TYPE_TEXTCOMPONENT_HPP
