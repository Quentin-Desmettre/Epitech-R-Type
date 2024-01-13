//
// Created by qdesmettre on 13/01/24.
//

#ifndef R_TYPE_DIFFICULTYCOMPONENT_HPP
#define R_TYPE_DIFFICULTYCOMPONENT_HPP

#include "aecs/AbstractComponent.hpp"

namespace rtype
{

    class DifficultyComponent : public aecs::AbstractComponent
    {
      public:
        DifficultyComponent() = default;
        ~DifficultyComponent() override = default;

        const char *getName() const override
        {
            return "DifficultyComponent";
        }
        [[nodiscard]] std::vector<std::byte> encode() const override;
        void decode(const std::vector<std::byte> &encoded) override;

        float difficulty = 1;
    };
} // namespace rtype

#endif // R_TYPE_DIFFICULTYCOMPONENT_HPP
