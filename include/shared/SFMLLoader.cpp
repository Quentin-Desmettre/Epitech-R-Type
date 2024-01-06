//
// Created by qdesmettre on 06/01/24.
//

#include "SFMLLoader.hpp"

std::map<std::string, sf::Texture> rtype::SFMLLoader::_textures;
std::map<std::string, sf::Font> rtype::SFMLLoader::_fonts;

sf::Font *rtype::SFMLLoader::loadFont(const std::string &font)
{
    if (_fonts.find(font) == _fonts.end()) {
        sf::Font newFont;
        if (!newFont.loadFromFile(font))
            return nullptr;
        _fonts[font] = newFont;
    }
    return &_fonts[font];
}

sf::Texture *rtype::SFMLLoader::loadTexture(const std::string &texture)
{
    if (_textures.find(texture) == _textures.end()) {
        sf::Texture newTexture;
        if (!newTexture.loadFromFile(texture))
            throw std::runtime_error("Cannot load texture: " + texture);
        _textures[texture] = newTexture;
    }
    return &_textures[texture];
}
