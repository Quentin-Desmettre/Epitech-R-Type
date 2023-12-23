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
        PlayerComponent() = default;
        ~PlayerComponent() override = default;
        [[nodiscard]] std::string encode() const override;
        void decode(const std::string &encoded) override;
        float timeSinceLastShoot = 0;
        float timeInShift = 0;
    };
} // namespace rtype

#endif // R_TYPE_PLAYERCOMPONENT_HPP
