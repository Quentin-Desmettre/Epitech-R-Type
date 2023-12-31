//
// Created by edo on 29/12/23.
//

#ifndef R_TYPE_DAMAGESOUNDSYSTEM_HPP
#define R_TYPE_DAMAGESOUNDSYSTEM_HPP

#include "aecs/SystemBase.hpp"
#include <SFML/Audio.hpp>
#include "aecs/World.hpp"

namespace rtype
{
    class DamageSoundSystem : public aecs::ALogicSystem
    {
      public:
        DamageSoundSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities);

        ~DamageSoundSystem() override = default;

        aecs::EntityChanges update(aecs::UpdateParams &updateParams) override;

        void onEntityRemoved(const aecs::EntityPtr &entity) override;

      private:
        sf::SoundBuffer _soundBuffer;
        sf::Sound _sound;
    };
} // namespace rtype


#endif // R_TYPE_DAMAGESOUNDSYSTEM_HPP
