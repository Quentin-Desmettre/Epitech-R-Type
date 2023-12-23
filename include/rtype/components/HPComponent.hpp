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
        explicit HPComponent(float hp) :
            hp(hp){};
        HPComponent() = default;
        [[nodiscard]] std::string encode() const override;
        void decode(const std::string &encoded) override;
        ~HPComponent() override = default;
        float hp = 0;
    };
} // namespace rtype

#endif // R_TYPE_HPCOMPONENT_HPP
