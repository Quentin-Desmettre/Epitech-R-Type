//
// Created by qdesmettre on 05/12/23.
//

#include "rtype/components/TextComponent.hpp"

rtype::TextComponent::TextComponent(const std::string &text, int fontSize, sf::Vector2f pos, sf::Color color, int zIndex) :
    zIndex(zIndex)
{
    if(!_font.loadFromFile("assets/fonts/Minecraft.ttf"))
        throw std::runtime_error("Cannot load font: assets/fonts/Minecraft.ttf");
    _text.setFont(_font);
    _text.setString(text);
    _text.setCharacterSize(fontSize);
    _text.setFillColor(color);
    sf::FloatRect textRect = _text.getLocalBounds();
    _text.setOrigin(textRect.left + textRect.width / 2.0f,
        textRect.top + textRect.height / 2.0f);
    _text.setPosition(pos);
}
