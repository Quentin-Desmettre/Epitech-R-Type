//
// Created by qdesmettre on 05/12/23.
//

#ifndef R_TYPE_RENDERSYSTEM_HPP
#define R_TYPE_RENDERSYSTEM_HPP

#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "aecs/SystemBase.hpp"
#include "aecs/World.hpp"
#include "rtype/components/PositionComponent.hpp"
#include "rtype/components/ShaderComponent.hpp"
#include "rtype/components/SpriteComponent.hpp"
#include "rtype/components/TextComponent.hpp"
#include <set>

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

        aecs::RenderInputs render() override;
        [[nodiscard]] bool isOpen() const override;
        void close() override;

      private:
        void _sortEntities();

        void _flushBuffers();
        void addEntity(const aecs::EntityPtr &entity);
        void deleteEntity(const aecs::EntityPtr &entity);
        void drawSprite(const aecs::EntityPtr &entity);
        void drawText(const aecs::EntityPtr &entity);
        void drawHealthBar(const aecs::EntityPtr &entity);
        void drawXPBar(const aecs::EntityPtr &entity);
        void drawBar(float width, float percentage, sf::Color color, sf::Vector2f position);

        std::vector<aecs::EntityPtr> _sortedEntities;
        std::vector<aecs::EntityPtr> _entitiesToDelete;
        std::vector<aecs::EntityPtr> _entitiesToAdd;

        sf::RenderWindow _window;
    };
} // namespace rtype

#endif // R_TYPE_RENDERSYSTEM_HPP
