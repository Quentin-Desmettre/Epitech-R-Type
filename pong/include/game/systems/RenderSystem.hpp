//
// Created by qdesmettre on 05/12/23.
//

#ifndef R_TYPE_RENDERSYSTEM_HPP
#define R_TYPE_RENDERSYSTEM_HPP

#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SystemBase.hpp"
#include "World.hpp"
#include "SpriteComponent.hpp"
#include <set>

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
        void deleteEntity(std::size_t entityId);
        void drawSprite(const aecs::EntityPtr &entity);
        void drawText(const aecs::EntityPtr &entity);

        std::vector<aecs::EntityPtr> _sortedEntities;
        std::vector<std::size_t> _entitiesToDelete;
        std::vector<aecs::EntityPtr> _entitiesToAdd;

        sf::RenderWindow _window;
};

#endif // R_TYPE_RENDERSYSTEM_HPP
