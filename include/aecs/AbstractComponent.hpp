//
// Created by qdesmettre on 04/12/23.
//

#ifndef R_TYPE_ABSTRACTCOMPONENT_HPP
#define R_TYPE_ABSTRACTCOMPONENT_HPP

#include <cstddef>
#include <string>
namespace aecs
{
    class AbstractComponent
    {
      public:
        virtual ~AbstractComponent() = 0;
        int id;

        virtual std::string encode() const { return ""; };
        virtual void decode(const std::string &str) {};

    };
} // namespace aecs


#endif // R_TYPE_ABSTRACTCOMPONENT_HPP
