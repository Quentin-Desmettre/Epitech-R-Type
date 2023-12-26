//
// Created by edo on 19/12/23.
//

#ifndef R_TYPE_MONSTERCOMPONENT_HPP
#define R_TYPE_MONSTERCOMPONENT_HPP
#include "aecs/AbstractComponent.hpp"

namespace rtype
{
    class MonsterComponent : public aecs::AbstractComponent
    {
      public:
        explicit MonsterComponent(bool lil = false) :
            _lil(lil)
        {
            id = 7;
        };
        ~MonsterComponent() override = default;
        float timeSinceLastShoot = 0;
        bool _lil = false;
        [[nodiscard]] std::vector<std::byte> encode() const override;
        void decode(const std::vector<std::byte> &encoded) override;
    };
} // namespace rtype

#endif // R_TYPE_MONSTERCOMPONENT_HPP
