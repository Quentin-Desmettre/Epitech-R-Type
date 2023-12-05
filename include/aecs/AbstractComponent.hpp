//
// Created by qdesmettre on 04/12/23.
//

#ifndef R_TYPE_ABSTRACTCOMPONENT_HPP
#define R_TYPE_ABSTRACTCOMPONENT_HPP

#include <cstddef>

namespace aecs
{
    class AbstractComponent
    {
      public:
        virtual ~AbstractComponent() = 0;
    };
} // namespace aecs

#endif // R_TYPE_ABSTRACTCOMPONENT_HPP
