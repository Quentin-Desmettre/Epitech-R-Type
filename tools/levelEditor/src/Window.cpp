/*
** EPITECH PROJECT, 2024
** levelEditor
** File description:
** Window
*/

#include "Window.hpp"
#include "Editor.hpp"
#include <cmath>

Window::Window() :
    _window(sf::VideoMode(1920, 1080), "Level Editor")
{
    _window.setFramerateLimit(60);
    _view = _window.getDefaultView();

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

        drawBackground();
        drawOutlines();
        drawMap(editor);
        if (editor.getMode() == ADD)
            drawSelected(editor.getSelectedBlock());

        _window.setView(_view);
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
            if (_event.key.code == sf::Keyboard::Right)
                _view.move(15, 0);
            if (_event.key.code == sf::Keyboard::Left && _view.getCenter().x > _view.getSize().x / 2)
                _view.move(-15, 0);
        }
    }
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(_window);

        if (mousePos.x < 0 || mousePos.y < 0 || (uint)mousePos.x > _window.getSize().x ||
            (uint)mousePos.y > _window.getSize().y)
            return;
        mousePos.x += _view.getCenter().x - _view.getSize().x / 2;
        editor.click(mousePos);
    }
}

void Window::drawMap(const Editor &editor)
{
    sf::Sprite sprite;
    auto &map = editor.getMap();

    for (std::size_t i = 0; i < map.size(); i++) {
        const Line &line = map[i];
        for (auto &[j, block] : line) {
            if (block == EMPTY)
                continue;
            sprite.setTexture(getTexture(block));
            sprite.setPosition(j * BLOCK_SIZE, i * BLOCK_SIZE);
            _window.draw(sprite);
        }
    }
}

void Window::drawBackground()
{
    sf::Vector2f pos = _view.getCenter() - _view.getSize() / 2.0f;

    _back.setPosition(pos);
    _window.draw(_back);
}

void Window::drawOutlines()
{
    sf::Vector2f pos = _view.getCenter() - _view.getSize() / 2.0f;
    sf::Vector2f size = _view.getSize();

    pos.x = std::floor(pos.x / BLOCK_SIZE) * BLOCK_SIZE;
    pos.y = std::floor(pos.y / BLOCK_SIZE) * BLOCK_SIZE;
    size.x = std::ceil(size.x / BLOCK_SIZE) * BLOCK_SIZE;
    size.y = std::ceil(size.y / BLOCK_SIZE) * BLOCK_SIZE;

    for (float i = pos.x; i < pos.x + size.x + 1; i += BLOCK_SIZE) {
        for (float j = pos.y; j < pos.y + size.y - BLOCK_SIZE; j += BLOCK_SIZE) {
            _outline.setPosition(i, j);
            _window.draw(_outline);
        }
    }
}

void Window::drawSelected(const Block selectedBlock)
{
    sf::Sprite sprite;
    sf::Vector2i mousePos = sf::Mouse::getPosition(_window);

    mousePos.x += _view.getCenter().x - _view.getSize().x / 2;
    mousePos /= BLOCK_SIZE;
    if (mousePos.x < 0 || mousePos.y < 0 || mousePos.y > 10)
        return;

    sprite.setTexture(getTexture(selectedBlock));
    sprite.setPosition(mousePos.x * BLOCK_SIZE, mousePos.y * BLOCK_SIZE);
    sprite.setColor(sf::Color(255, 255, 255, 75));
    _window.draw(sprite);
}

sf::Texture &Window::getTexture(const Block block)
{
    if (_textures.find(block) == _textures.end())
        _textures[block].loadFromFile(blockPaths.at(block));
    return _textures[block];
}
