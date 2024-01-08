//
// Created by qdesmettre on 05/12/23.
//

#include "rtype/components/SpriteComponent.hpp"
#include "shared/SFMLLoader.hpp"

rtype::SpriteComponent::SpriteComponent(const std::string &path, sf::Vector2f size, sf::IntRect rect, int zIndex, int setOrigin) :
    _size(size),
    zIndex(zIndex)
{
    sf::Texture *texture = SFMLLoader::loadTexture(path);

    if (!texture)
        throw std::runtime_error("Cannot load texture: " + path);
    texture->setRepeated(true);
    sprite.setTexture(*texture);
    if (rect.width == 0 && rect.height == 0) {
        rect.width = texture->getSize().x;
        rect.height = texture->getSize().y;
    }
    // scale sprite
    sf::Vector2<unsigned int> spSize = texture->getSize();
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

    if (setOrigin == 1) {
        sprite.setOrigin(sprite.getGlobalBounds().width / 2, sprite.getGlobalBounds().height / 2);
    }
}
