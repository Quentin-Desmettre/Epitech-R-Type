/*
** EPITECH PROJECT, 2024
** levelEditor
** File description:
** Window
*/

#include "Window.hpp"
#include "Editor.hpp"

Window::Window() :
    _window(sf::VideoMode(1920, 1080), "Level Editor")
{
    _window.setFramerateLimit(60);

    _backTexture.loadFromFile("assets/background.png");
    _back.setTexture(_backTexture);
    _back.setScale(7.06, 7.06);

    _outline.setSize(sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE));
    _outline.setFillColor(sf::Color::Transparent);
    _outline.setOutlineColor(sf::Color(255, 255, 255, 20));
    _outline.setOutlineThickness(2);
}

void Window::run(Editor &editor)
{
    while (_window.isOpen()) {
        handleEvents(editor);
        _window.clear();
        _window.draw(_back);
        drawMap(editor);
        _window.display();
    }
}

void Window::handleEvents(Editor &editor)
{
    while (_window.pollEvent(_event)) {
        if (_event.type == sf::Event::Closed ||
            (_event.type == sf::Event::KeyPressed && _event.key.code == sf::Keyboard::Escape))
            _window.close();
        if (_event.type == sf::Event::KeyPressed) {
            if (_event.key.code == sf::Keyboard::A)
                editor.switchMode(ADD);
            if (_event.key.code == sf::Keyboard::D)
                editor.switchMode(DELETE);
            if (_event.key.code == sf::Keyboard::Enter)
                editor.saveMap();
        }
    }
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        editor.click(sf::Mouse::getPosition(_window));
}

void Window::drawMap(const Editor &editor)
{
    sf::Sprite sprite;
    auto &map = editor.getMap();

    for (std::size_t i = 0; i < map.size(); i++) {
        const Line &line = map[i];
        std::vector<std::size_t> visited;
        for (auto &[j, block] : line) {
            if (block == EMPTY)
                continue;
            sprite.setTexture(getTexture(block));
            sprite.setPosition(j * BLOCK_SIZE, i * BLOCK_SIZE);
            _window.draw(sprite);
            visited.push_back(j);
        }
        drawOutlines(visited, i);
    }
}

void Window::drawOutlines(const std::vector<std::size_t> &visited, std::size_t i)
{
    for (std::size_t j = 0; j < 20; j++) {
        if (std::find(visited.begin(), visited.end(), j) == visited.end()) {
            _outline.setPosition(j * BLOCK_SIZE, i * BLOCK_SIZE);
            _window.draw(_outline);
        }
    }
}

sf::Texture &Window::getTexture(const Block block)
{
    if (_textures.find(block) == _textures.end())
        _textures[block].loadFromFile(blockPaths.at(block));
    return _textures[block];
}
