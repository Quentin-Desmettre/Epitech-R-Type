//
// Created by edo on 29/12/23.
//
#include "rtype/components/MusicComponent.hpp"

rtype::MusicComponent::MusicComponent(const std::string &path, float volume, bool loop)
{
    music.openFromFile(path);
    music.setVolume(volume);
    music.setLoop(loop);
    music.play();
}
