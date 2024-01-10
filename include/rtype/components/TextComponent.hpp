//
// Created by qdesmettre on 05/12/23.
//

#ifndef R_TYPE_TEXTCOMPONENT_HPP
#define R_TYPE_TEXTCOMPONENT_HPP

#include "aecs/AbstractComponent.hpp"
#include <SFML/Graphics.hpp>
#include <functional>

namespace rtype
{
    class TextComponent : public aecs::AbstractComponent
    {
      public:
        explicit TextComponent(const std::string &text, int fontSize = 30, sf::Color color = sf::Color::White, bool setCenter = 0, int zIndex = 0, std::function<void()> &&onClick = nullptr);
        ~TextComponent() override = default;
        void onOver();
        void removeOver();
        void onPress();

        sf::Text _text;
        sf::Font _font;
        std::function<void()> onClick;
        int zIndex;

        const char *getName() const override
        {
            return "TextComponent";
        };

      private:
        int _character_size;
        int _over_size;
        int _default_size;
        bool _setCenter;

    };
} // namespace rtype

#endif // R_TYPE_TEXTCOMPONENT_HPP
