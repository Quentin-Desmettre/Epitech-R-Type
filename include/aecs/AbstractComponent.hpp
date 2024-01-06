//
// Created by qdesmettre on 04/12/23.
//

#ifndef R_TYPE_ABSTRACTCOMPONENT_HPP
#define R_TYPE_ABSTRACTCOMPONENT_HPP

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace aecs
{
    class AbstractComponent
    {
      public:
        virtual ~AbstractComponent() = 0;
        [[nodiscard]] virtual const char *getName() const = 0;

        [[nodiscard]] virtual std::vector<std::byte> encode() const;
        virtual void decode(const std::vector<std::byte> &data);
    };
} // namespace aecs

#endif // R_TYPE_ABSTRACTCOMPONENT_HPP
