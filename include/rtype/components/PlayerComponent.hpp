//
// Created by edo on 06/12/23.
//

#ifndef R_TYPE_PLAYERCOMPONENT_HPP
#define R_TYPE_PLAYERCOMPONENT_HPP

#include "aecs/AbstractComponent.hpp"

namespace rtype
{
    class PlayerComponent : public aecs::AbstractComponent
    {
      public:
        explicit PlayerComponent();
        ~PlayerComponent() override;

        [[nodiscard]] std::vector<std::byte> encode() const override;
        void decode(const std::vector<std::byte> &encoded) override;

        void unUsePlayerId() const;

        float timeSinceLastShoot = 0;
        float timeInShift = 0;
        std::uint8_t playerId = 0;

        const char *getName() const override
        {
            return "PlayerComponent";
        };

      private:
        static std::uint8_t _playerIds[255];
        static bool _playerIdsUsed[255];
    };
} // namespace rtype

#endif // R_TYPE_PLAYERCOMPONENT_HPP
