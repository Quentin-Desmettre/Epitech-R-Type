//
// Created by Clément Vandeville on 14/01/2024.
//

#include "TagComponent.hpp"

TagComponent::TagComponent(std::string tag) : tag(std::move(tag))
{
}

std::vector<std::byte> TagComponent::encode() const
{
    return AbstractComponent::encode();
}

void TagComponent::decode(const std::vector<std::byte> &encoded)
{
    AbstractComponent::decode(encoded);
}
