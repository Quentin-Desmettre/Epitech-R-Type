//
// Created by edo on 06/12/23.
//

#ifndef R_TYPE_PLAYERCOMPONENT_HPP
#define R_TYPE_PLAYERCOMPONENT_HPP

#include "AbstractComponent.hpp"

class PlayerComponent : public aecs::AbstractComponent
{
    public:
        explicit PlayerComponent(bool isAI = false);
        ~PlayerComponent() override;

        [[nodiscard]] std::vector<std::byte> encode() const override;
        void decode(const std::vector<std::byte> &encoded) override;

        void unUsePlayerId() const;

        std::uint8_t playerId = 0;

        const char *getName() const override
        {
            return "PlayerComponent";
        };

        bool isAI = false;
        int score = 0;

    private:
        static std::uint8_t _playerIds[255];
        static bool _playerIdsUsed[255];

};

#endif // R_TYPE_PLAYERCOMPONENT_HPP
