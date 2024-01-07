//
// Created by Clément Vandeville on 07/01/2024.
//

#include "rtype/components/TagComponent.hpp"

namespace rtype {

    TagComponent::TagComponent(std::string tag) :
            tag(std::move(tag))
    {}
} // namespace rtype