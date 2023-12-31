//
// Created by edo on 12/12/23.
//

#ifndef R_TYPE_HPCOMPONENT_HPP
#define R_TYPE_HPCOMPONENT_HPP
#include "aecs/AbstractComponent.hpp"

namespace rtype
{
    class HPComponent : public aecs::AbstractComponent
    {
      public:
        explicit HPComponent(float hp = 10) :
            hp(hp){};
        [[nodiscard]] std::vector<std::byte> encode() const override;
        void decode(const std::vector<std::byte> &encoded) override;
        ~HPComponent() override = default;
        float hp = 0;
    };
} // namespace rtype

#endif // R_TYPE_HPCOMPONENT_HPP
