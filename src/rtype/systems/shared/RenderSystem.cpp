//
// Created by qdesmettre on 05/12/23.
//

#include "rtype/systems/shared/RenderSystem.hpp"
#include "rtype/components/PositionComponent.hpp"
#include "rtype/components/ShaderComponent.hpp"
#include "rtype/components/SpriteComponent.hpp"

rtype::RenderSystem::RenderSystem(aecs::World &world,
                                  const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
    ARenderSystem(world, entities, {typeid(rtype::SpriteComponent), typeid(rtype::PositionComponent)}),
    _window(sf::VideoMode(1088, 640), "R-Type")
{
}

void rtype::RenderSystem::onEntityAdded(const aecs::EntityPtr &entity)
{
    ARenderSystem::onEntityAdded(entity);

    if (entity->hasComponents(_componentsNeeded)) {
        _sortedEntities.push_back(entity);
        _sortEntities();
    }
}

void rtype::RenderSystem::onEntityRemoved(const aecs::EntityPtr &entity)
{
    ARenderSystem::onEntityRemoved(entity);

    if (entity->hasComponents(_componentsNeeded)) {
        _sortedEntities.erase(std::remove(_sortedEntities.begin(), _sortedEntities.end(), entity),
                              _sortedEntities.end());
    }
}

void rtype::RenderSystem::onEntityModified(const aecs::EntityPtr &entity)
{
    ARenderSystem::onEntityModified(entity);

    if (entity->hasComponents(_componentsNeeded)) {
        if (std::find(_sortedEntities.begin(), _sortedEntities.end(), entity) == _sortedEntities.end())
            _sortedEntities.push_back(entity);
        _sortEntities();
    } else {
        _sortedEntities.erase(std::remove(_sortedEntities.begin(), _sortedEntities.end(), entity),
                              _sortedEntities.end());
    }
}

void rtype::RenderSystem::_sortEntities()
{
    std::sort(_sortedEntities.begin(), _sortedEntities.end(), [](const auto &a, const auto &b) {
        int indexA = ((aecs::EntityPtr)a)->getComponent<SpriteComponent>().zIndex;
        int indexB = ((aecs::EntityPtr)b)->getComponent<SpriteComponent>().zIndex;
        return indexA < indexB;
    });
}

aecs::ClientInputs rtype::RenderSystem::render()
{
    aecs::ClientInputs inputs;

    // Poll events
    sf::Event event{};
    while (_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            _window.close();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            _window.close();
    }
    for (int i = 0; i < sf::Keyboard::KeyCount; i++) {
        if (sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(i)))
            inputs.push_back(static_cast<aecs::RenderInput>(i));
    }

    // Render
    _window.clear(sf::Color(63, 63, 63));
    for (auto &entity : _sortedEntities) {
        auto &sprite = entity->getComponent<rtype::SpriteComponent>();
        auto &pos = entity->getComponent<PositionComponent>();
        sprite.sprite.setPosition(pos.x, pos.y);
        if (entity->hasComponent<ShaderComponent>()) {
            auto &shader = entity->getComponent<ShaderComponent>();
            _window.draw(sprite.sprite, shader.shader.get());
        } else
            _window.draw(sprite.sprite);
    }
    _window.display();
    return inputs;
}

bool rtype::RenderSystem::isOpen() const
{
    return _window.isOpen();
}
