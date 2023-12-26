//
// Created by edo on 21/12/23.
//

#ifndef R_TYPE_SHADERCOMPONENT_HPP
#define R_TYPE_SHADERCOMPONENT_HPP

#include "aecs/AbstractComponent.hpp"
#include <SFML/Graphics.hpp>
#include <memory>

namespace rtype
{
    class ShaderComponent : public aecs::AbstractComponent
    {
      public:
        explicit ShaderComponent(std::shared_ptr<sf::Shader> shader) :
            shader(shader){};
        ~ShaderComponent() override = default;

        std::shared_ptr<sf::Shader> shader;
    };
} // namespace rtype

#endif // R_TYPE_SHADERCOMPONENT_HPP
