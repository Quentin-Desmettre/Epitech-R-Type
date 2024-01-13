//
// Created by Clément Vandeville on 07/01/2024.
//

#include "TextComponent.hpp"

#include <utility>

sf::Font TextComponent::font;
bool TextComponent::loadFont = font.loadFromFile("assets/fonts/TT Lakes Neue Trial Black.ttf");

TextComponent::TextComponent(std::string string, int fontSize, sf::Color color, int zIndex) :
        zIndex(zIndex),
        _string(std::move(string)),
        _fontSize(fontSize),
        _color(color)
{
    text.setString(_string);
    text.setCharacterSize(_fontSize);
    text.setFillColor(_color);

    if (!loadFont) return;
    text.setFont(font);
}
