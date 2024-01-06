//
// Created by edo on 29/12/23.
//

#ifndef R_TYPE_MUSICCOMPONENT_HPP
#define R_TYPE_MUSICCOMPONENT_HPP

#include "aecs/AbstractComponent.hpp"
#include <SFML/Audio.hpp>
namespace rtype
{
    class MusicComponent : public aecs::AbstractComponent
    {
      public:
        explicit MusicComponent(const std::string& path, float volume = 100, bool loop = true);
        ~MusicComponent() override = default;

        sf::Music music;

        const char *getName() const override {
            return "MusicComponent";
        };
    };
} // namespace rtype

#endif // R_TYPE_MUSICCOMPONENT_HPP
