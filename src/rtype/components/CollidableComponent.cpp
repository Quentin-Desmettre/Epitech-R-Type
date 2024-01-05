//
// Created by qdesmettre on 05/01/24.
//

#include "rtype/components/CollidableComponent.hpp"

namespace rtype
{
    CollidableComponent::CollidableComponent(int priority) :
        _priority(priority)
    {
    }

    int CollidableComponent::getPriority() const
    {
        return _priority;
    }

    const char *CollidableComponent::getName() const
    {
        return "CollidableComponent";
    }

} // namespace rtype
