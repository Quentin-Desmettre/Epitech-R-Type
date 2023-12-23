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
        explicit DamageCollisionComponent(int team, float damage, ObjectType type = OTHER) :
            team(team),
            damage(damage),
            type(type){};
        DamageCollisionComponent() = default;
        [[nodiscard]] std::string encode() const override;
        void decode(const std::string &encoded) override;
        ~DamageCollisionComponent() override = default;
        int team = 0;
        float damage = 0;
        ObjectType type = OTHER;
        float invulnerability = 0;
    };
} // namespace rtype

#endif // R_TYPE_DAMAGECOLLISIONCOMPONENT_HPP
