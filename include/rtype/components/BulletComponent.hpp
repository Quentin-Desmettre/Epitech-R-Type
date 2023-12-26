//
// Created by edo on 10/12/23.
//

#ifndef R_TYPE_BULLETCOMPONENT_HPP
#define R_TYPE_BULLETCOMPONENT_HPP

#include "aecs/AbstractComponent.hpp"

namespace rtype
{
    class BulletComponent : public aecs::AbstractComponent
    {
      public:
        BulletComponent()
        {
            id = 2;
        };
        ~BulletComponent() override = default;
    };
} // namespace rtype

#endif // R_TYPE_BULLETCOMPONENT_HPP
