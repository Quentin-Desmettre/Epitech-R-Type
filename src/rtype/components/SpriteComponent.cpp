//
// Created by qdesmettre on 05/12/23.
//

#include "rtype/components/SpriteComponent.hpp"

rtype::SpriteComponent::SpriteComponent(const std::string &path, sf::Vector2f size, sf::IntRect rect) :
    _size(size)
{
    if (!_texture.loadFromFile(path))
        throw std::runtime_error("Cannot load texture: " + path);
    _texture.setRepeated(true);
    sprite.setTexture(_texture);
    if (rect.width == 0 && rect.height == 0) {
        rect.width = _texture.getSize().x;
        rect.height = _texture.getSize().y;
    }
    // scale sprite
    sf::Vector2<unsigned int> spSize = _texture.getSize();
    if ((unsigned)rect.width < spSize.x) {
        spSize.x = rect.width;
    }
    if ((unsigned)rect.height < spSize.y) {
        spSize.y = rect.height;
    }

    sprite.setTextureRect(rect);
    if (size.x != 0 && size.y != 0) {
        sprite.setScale(size.x / spSize.x, size.y / spSize.y);
    }
}
