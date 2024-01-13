//
// Created by edo on 11/01/24.
//

#ifndef R_TYPE_BOSSCOMPONENT_HPP
#define R_TYPE_BOSSCOMPONENT_HPP

#include "aecs/AbstractComponent.hpp"

namespace rtype
{
    class BossComponent : public aecs::AbstractComponent
    {
      public:
        BossComponent() = default;
        ~BossComponent() override = default;

        const char *getName() const override
        {
            return "BossComponent";
        };
    };
} // namespace rtype

#endif // R_TYPE_BOSSCOMPONENT_HPP
