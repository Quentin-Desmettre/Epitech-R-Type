//
// Created by edo on 12/12/23.
//

#ifndef R_TYPE_COLLIDABLECOMPONENT_HPP
#define R_TYPE_COLLIDABLECOMPONENT_HPP

#include "aecs/AbstractComponent.hpp"

namespace rtype
{
    class CollidableComponent : public aecs::AbstractComponent
    {
      public:
        explicit CollidableComponent(int priority = 0);

        ~CollidableComponent() override = default;
        [[nodiscard]] const char *getName() const override;

        [[nodiscard]] int getPriority() const;

      private:
        int _priority;
    };
} // namespace rtype

#endif // R_TYPE_COLLIDABLECOMPONENT_HPP
