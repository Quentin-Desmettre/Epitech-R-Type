//
// Created by qdesmettre on 05/12/23.
//

#ifndef R_TYPE_RENDERSYSTEM_HPP
#define R_TYPE_RENDERSYSTEM_HPP

#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "aecs/SystemBase.hpp"
#include "aecs/World.hpp"

namespace rtype
{
    class RenderSystem : public aecs::ARenderSystem
    {
      public:
        RenderSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities);
        ~RenderSystem() override = default;

        void onEntityAdded(const aecs::EntityPtr &entity) override;
        void onEntityRemoved(const aecs::EntityPtr &entity) override;
        void onEntityModified(const aecs::EntityPtr &entity) override;

        aecs::ClientInputs render() override;
        [[nodiscard]] bool isOpen() const override;

      private:
        void _sortEntities();

        std::vector<aecs::EntityPtr> _sortedEntities;
        sf::RenderWindow _window;
    };
} // namespace rtype

#endif // R_TYPE_RENDERSYSTEM_HPP
