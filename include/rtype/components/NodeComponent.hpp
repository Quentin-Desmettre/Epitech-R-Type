//
// Created by edo on 03/01/24.
//

#ifndef R_TYPE_NODECOMPONENT_HPP
#define R_TYPE_NODECOMPONENT_HPP

#include "aecs/AbstractComponent.hpp"

namespace rtype
{
    class NodeComponent : public aecs::AbstractComponent
    {
      public:
        explicit NodeComponent(float delay = 0.f) :
            delay(delay){};
        ~NodeComponent() override = default;
        const char *getName() const override
        {
            return "NodeComponent";
        };
        float delay = 0;
        float rotate = 0;

        [[nodiscard]] std::vector<std::byte> encode() const override;
        void decode(const std::vector<std::byte> &encoded) override;
    };
} // namespace rtype

#endif // R_TYPE_NODECOMPONENT_HPP
