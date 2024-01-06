/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Menu.cpp
*/

#include "shared/Menu.hpp"
#include "shared/SFMLLoader.hpp"

ButtonData::ButtonData(const std::string &text, const std::string &texturePath, std::function<void()> &&handler, sf::Vector2f pos, sf::IntRect rect)
{
    _pos = pos;
    _font.loadFromFile("assets/fonts/Minecraft.ttf");
    auto *texture = rtype::SFMLLoader::loadTexture(texturePath);
    if (texture)
        _sprite.setTexture(*texture);
    _sprite.setTextureRect(rect);
    sf::FloatRect spriteRect = _sprite.getGlobalBounds();
    _sprite.setPosition(_pos);
    _text.setFont(_font);
    _text.setString(text);
    _text.setCharacterSize(20);
    sf::FloatRect textRect = _text.getGlobalBounds();
    //center text in sprite:
    float textPosX = (spriteRect.width / 2) - (textRect.width / 2);
    float textPosY = (spriteRect.height / 2) - (textRect.height / 2);
    _text.setPosition(_pos.x + textPosX, _pos.y + textPosY);
    _handler = handler;
}

Menu::Menu(
    const std::vector<ButtonData> &buttons,
    const std::map<Input, std::function<void()>> &handlers,
    const std::vector<SystemPriority> &systems,
    std::function<void()> &&setup
)
{
    _buttons = buttons;
    _handlers = handlers;
    _systems = systems;
    _setup = setup;
}
