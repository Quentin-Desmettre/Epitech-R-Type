//
// Created by qdesmettre on 05/12/23.
//

#include "rtype/components/TextComponent.hpp"
#include "shared/SFMLLoader.hpp"

rtype::TextComponent::TextComponent(const std::string &text, int fontSize, sf::Color color, int zIndex, std::function<void()> &&onClick) :
    zIndex(zIndex)
{
    sf::Font *font = SFMLLoader::loadFont("assets/fonts/Minecraft.ttf");
    if (font)
        _text.setFont(*font);
    _text.setString(text);
    _text.setCharacterSize(fontSize);
    _text.setFillColor(color);
    this->onClick = onClick;
    this->zIndex = zIndex;
}
