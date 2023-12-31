//
// Created by qdesmettre on 04/12/23.
//

#include "aecs/AbstractComponent.hpp"

aecs::AbstractComponent::~AbstractComponent() = default;

std::vector<std::byte> aecs::AbstractComponent::encode() const
{
    return {};
}

void aecs::AbstractComponent::decode(const std::vector<std::byte> &data)
{
    (void)data;
}
