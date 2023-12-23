//
// Created by qdesmettre on 04/12/23.
//

#ifndef R_TYPE_ABSTRACTCOMPONENT_HPP
#define R_TYPE_ABSTRACTCOMPONENT_HPP

#include <cstddef>
#include <string>
#include <vector>
namespace aecs
{
    class AbstractComponent
    {
      public:
        virtual ~AbstractComponent() = 0;
        int id;

        [[nodiscard]] virtual std::vector<std::byte> encode() const { return {}; };
        virtual void decode(const std::vector<std::byte> &data) {};

    };
} // namespace aecs


#endif // R_TYPE_ABSTRACTCOMPONENT_HPP
