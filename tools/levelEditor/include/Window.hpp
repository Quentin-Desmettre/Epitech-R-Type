/*
** EPITECH PROJECT, 2024
** Epitech-R-Type
** File description:
** Window
*/

#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>

class Editor;
enum Block : char;

class Window
{
  public:
    Window();
    ~Window() = default;

    void run(Editor &editor);
    void drawMap(const Editor &editor);

  protected:
  private:
    void handleEvents(Editor &editor);
    void drawOutlines(const std::vector<std::size_t> &visized, std::size_t i);
    void drawSelected(const Block selectedBlock);
    sf::Texture &getTexture(const Block block);

    sf::RenderWindow _window;
    sf::Event _event;
    sf::Sprite _back;
    sf::Texture _backTexture;
    sf::RectangleShape _outline;
    std::map<Block, sf::Texture> _textures;
};
