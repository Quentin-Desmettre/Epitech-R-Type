//
// Created by qdesmettre on 05/12/23.
//

#include "rtype/components/SpriteComponent.hpp"

rtype::SpriteComponent::SpriteComponent(const std::string &path, sf::Vector2f size)
{
    if (!_texture.loadFromFile(path))
        throw std::runtime_error("Cannot load texture: " + path);
    _texture.setSmooth(true);
    sprite.setTexture(_texture);
    // scale sprite
    if (size.x != 0 && size.y != 0) {
        auto textureSize = _texture.getSize();
        sprite.setScale(size.x / textureSize.x, size.y / textureSize.y);
    }
}
