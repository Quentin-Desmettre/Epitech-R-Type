//
// Created by Clément Vandeville on 07/01/2024.
//

#ifndef R_TYPE_TAGCOMPONENT_HPP
#define R_TYPE_TAGCOMPONENT_HPP


#include "aecs/AbstractComponent.hpp"

namespace rtype {

    class TagComponent : public aecs::AbstractComponent {
        public:
            explicit TagComponent(std::string tag);

            ~TagComponent() override = default;

            const char *getName() const override {
                return "TagComponent";
            }

            std::vector<std::byte> encode() const override;
            void decode(const std::vector<std::byte> &data) override;

            std::string tag;
    };
} // namespace rtype

#endif //R_TYPE_TAGCOMPONENT_HPP
