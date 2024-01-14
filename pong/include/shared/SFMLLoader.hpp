//
// Created by qdesmettre on 06/01/24.
//

#ifndef R_TYPE_SFMLLOADER_HPP
#define R_TYPE_SFMLLOADER_HPP

#include <SFML/Graphics.hpp>
#include <map>

namespace rtype
{

    class SFMLLoader
    {
        public:
            static sf::Font *loadFont(const std::string &font);
            static sf::Texture *loadTexture(const std::string &texture);

        private:
            static std::map<std::string, sf::Texture> _textures;
            static std::map<std::string, sf::Font> _fonts;
    };
} // namespace rtype

#endif // R_TYPE_SFMLLOADER_HPP
