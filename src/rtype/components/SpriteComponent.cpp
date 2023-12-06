//
// Created by qdesmettre on 05/12/23.
//

#include "rtype/components/SpriteComponent.hpp"

rtype::SpriteComponent::SpriteComponent(const std::string &path, sf::Vector2f size, sf::IntRect rect):
    _size(size)
{
    if (!_texture.loadFromFile(path))
        throw std::runtime_error("Cannot load texture: " + path);
    _texture.setSmooth(true);
    sprite.setTexture(_texture);
    // scale sprite
    sf::Vector2<unsigned int> spSize = _texture.getSize();
    if (rect.width != 0 && rect.height != 0) {
        spSize = sf::Vector2<unsigned int>(rect.width, rect.height);
        sprite.setTextureRect(rect);
    }

    if (size.x != 0 && size.y != 0) {
        sprite.setScale(size.x / spSize.x, size.y / spSize.y);
    }
}
