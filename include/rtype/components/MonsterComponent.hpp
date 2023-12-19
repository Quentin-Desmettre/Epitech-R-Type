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
        explicit MonsterComponent(bool lil = false): _lil(lil) {};
        ~MonsterComponent() override = default;
        float timeSinceLastShoot = 0;
        float _lil;
    };
} // namespace rtype


#endif // R_TYPE_MONSTERCOMPONENT_HPP
