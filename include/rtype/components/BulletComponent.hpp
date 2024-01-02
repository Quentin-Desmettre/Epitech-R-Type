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
        explicit BulletComponent(bool big = false) :
            big(big){};
        ~BulletComponent() override = default;
        bool big = false;

        const char *getName() const override {
            return "BulletComponent";
        };


        [[nodiscard]] std::vector<std::byte> encode() const override;
        void decode(const std::vector<std::byte> &encoded) override;
    };
} // namespace rtype

#endif // R_TYPE_BULLETCOMPONENT_HPP
