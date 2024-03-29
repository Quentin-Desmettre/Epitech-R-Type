/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Menu.hpp
*/
#ifndef R_TYPE_MENU_HPP
#define R_TYPE_MENU_HPP

#include "SFML/Graphics.hpp"
#include "SystemBase.hpp"
#include <functional>
#include <iostream>
#include <map>
#include <vector>

enum Menus { MainMenu, GameMenu };

class ButtonData
{
    public:
        ButtonData(const std::string &text, const std::string &texturePath, std::function<void()> &&handler,
                   sf::Vector2f pos = {0, 0}, sf::IntRect rect = {-1, -1, -1, -1});
        ~ButtonData() = default;

    private:
        std::function<void()> _handler;
        sf::Text _text;
        sf::Sprite _sprite;
        sf::Vector2f _pos;
        sf::Font _font;
};

typedef int Input;

class Menu
{
    public:
        Menu(
                const std::vector<ButtonData> &buttons, const std::map<Input, std::function<void()>> &handlers,
                const std::vector<aecs::SystemData> &systems = {}, std::function<void()> &&setup = []() {
        });
        ~Menu() = default;
        Menu(const Menu &menu) = default;
        Menu &operator=(const Menu &menu) = default;

        std::vector<ButtonData> _buttons;
        std::map<Input, std::function<void()>> _handlers;
        std::vector<aecs::SystemData> _systems;
        std::function<void()> _setup;
};

#endif // R_TYPE_MENU_HPP
