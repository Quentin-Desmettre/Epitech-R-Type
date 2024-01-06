//
// Created by qdesmettre on 05/12/23.
//

#include "rtype/components/TextComponent.hpp"
#include "shared/SFMLLoader.hpp"

rtype::TextComponent::TextComponent(const std::string &text, int fontSize, sf::Color color, int zIndex) :
    zIndex(zIndex)
{
    sf::Font *font = SFMLLoader::loadFont("assets/fonts/Minecraft.ttf");
    if (font)
        _text.setFont(*font);
    _text.setString(text);
    _text.setCharacterSize(fontSize);
    _text.setFillColor(color);
    sf::FloatRect textRect = _text.getLocalBounds();
    _text.setOrigin(textRect.left + textRect.width / 2.0f,
        textRect.top + textRect.height / 2.0f);
}
