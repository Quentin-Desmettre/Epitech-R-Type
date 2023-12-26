//
// Created by edo on 12/12/23.
//

#ifndef R_TYPE_DAMAGECOLLISIONCOMPONENT_HPP
#define R_TYPE_DAMAGECOLLISIONCOMPONENT_HPP

#include "aecs/AbstractComponent.hpp"

namespace rtype
{
    class DamageCollisionComponent : public aecs::AbstractComponent
    {
      public:
        enum Team {
            PLAYER,
            ENEMY,
        };
        enum ObjectType {
            SM_BULLET,
            LG_BULLET,
            OTHER,
        };
        explicit DamageCollisionComponent(int team = 0, float damage = 0.f, ObjectType type = OTHER) :
            team(team),
            damage(damage),
            type(type){};
        [[nodiscard]] std::vector<std::byte> encode() const override;
        void decode(const std::vector<std::byte> &encoded) override;
        ~DamageCollisionComponent() override = default;
        int team = 0;
        float damage = 0;
        ObjectType type = OTHER;
        float invulnerability = 0;
    };
} // namespace rtype

#endif // R_TYPE_DAMAGECOLLISIONCOMPONENT_HPP
