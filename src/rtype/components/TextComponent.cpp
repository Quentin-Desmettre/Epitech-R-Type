//
// Created by qdesmettre on 05/12/23.
//

#include "rtype/components/TextComponent.hpp"
#include "shared/SFMLLoader.hpp"

rtype::TextComponent::TextComponent(const std::string &text, int fontSize, sf::Color color, bool setCenter, int zIndex,
                                    std::function<void()> &&onClick) :
    zIndex(zIndex),
    _character_size(fontSize),
    _over_size(fontSize + 30),
    _default_size(fontSize),
    _setCenter(setCenter)
{
    sf::Font *font = SFMLLoader::loadFont("assets/fonts/Minecraft.ttf");
    if (font)
        _text.setFont(*font);
    _text.setString(text);
    _text.setCharacterSize(fontSize);
    _text.setFillColor(color);
    if (_setCenter) {
        sf::FloatRect textRect = _text.getGlobalBounds();
        _text.setOrigin(textRect.width / 2, textRect.height / 2);
    }
    this->onClick = onClick;
    this->zIndex = zIndex;
}

void rtype::TextComponent::onOver()
{
    if (_character_size == _over_size)
        return;
    _character_size = _over_size;

    _text.setCharacterSize(_character_size);
    if (_setCenter) {
        sf::FloatRect textRect = _text.getGlobalBounds();
        _text.setOrigin(textRect.width / 2, textRect.height / 2);
    }
}

void rtype::TextComponent::onPress()
{
    if (_text.getOutlineThickness() != 0.0)
        return;
    _character_size = _over_size;

    _text.setOutlineColor(sf::Color::Black);
    _text.setOutlineThickness(2);
    _text.setCharacterSize(_character_size);
    if (_setCenter) {
        sf::FloatRect textRect = _text.getGlobalBounds();
        _text.setOrigin(textRect.width / 2, textRect.height / 2);
    }
}

void rtype::TextComponent::removeOver()
{
    if (_character_size == _default_size)
        return;

    _character_size = _default_size;
    _text.setOutlineThickness(0);

    _text.setCharacterSize(_character_size);
    if (_setCenter) {
        sf::FloatRect textRect = _text.getGlobalBounds();
        _text.setOrigin(textRect.width / 2, textRect.height / 2);
    }
}
