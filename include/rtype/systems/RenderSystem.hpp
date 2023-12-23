//
// Created by qdesmettre on 05/12/23.
//

#ifndef R_TYPE_RENDERSYSTEM_HPP
#define R_TYPE_RENDERSYSTEM_HPP

#include "aecs/SystemBase.hpp"
#include "aecs/World.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

namespace rtype
{
    class RenderSystem : public aecs::ARenderSystem
    {
      public:
        RenderSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities);
        ~RenderSystem() override = default;

        aecs::ClientInputs render() override;
        [[nodiscard]] bool isOpen() const override;

      private:
        sf::RenderWindow _window;
    };
} // namespace rtype

#endif // R_TYPE_RENDERSYSTEM_HPP
