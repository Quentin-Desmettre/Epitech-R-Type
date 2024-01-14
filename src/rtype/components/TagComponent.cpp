//
// Created by Clément Vandeville on 07/01/2024.
//

#include "rtype/components/TagComponent.hpp"
#include "shared/PacketBuilder.hpp"

namespace rtype {

    TagComponent::TagComponent(std::string tag) :
            tag(std::move(tag))
    {}

    std::vector<std::byte> TagComponent::encode() const
    {
        PacketBuilder pb;
        pb << tag;
        return pb.getData();
    }

    void TagComponent::decode(const std::vector<std::byte> &encoded)
    {
        PacketBuilder pb;
        pb << encoded;
        pb >> tag;
    }

} // namespace rtype
