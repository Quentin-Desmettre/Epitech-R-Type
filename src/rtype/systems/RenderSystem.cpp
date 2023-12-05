//
// Created by qdesmettre on 05/12/23.
//

#include "rtype/systems/RenderSystem.hpp"
#include "rtype/components/SpriteComponent.hpp"
#include "rtype/components/PositionComponent.hpp"

rtype::RenderSystem::RenderSystem(aecs::World &world,
                                  const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
    _world(world),
    _window(sf::VideoMode(800, 600), "R-Type")
{
    for (auto &[_id, entity] : entities)
        RenderSystem::onEntityAdded(entity);
}

std::vector<rtype::RenderSystem::RenderInput> rtype::RenderSystem::render()
{
    std::vector<RenderInput> inputs;

    // Poll events
    sf::Event event{};
    while (_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            _window.close();
        else if (event.type == sf::Event::KeyPressed)
            inputs.emplace_back(event.key.code);
    }

    // Render
    _window.clear();
    for (auto &[id, entity] : _entitiesMap) {
        auto &sprite = entity->getComponent<rtype::SpriteComponent>();
        auto &pos = entity->getComponent<PositionComponent>();
        sprite.sprite.setPosition(pos.x, pos.y);
        _window.draw(sprite.sprite);
    }
    _window.display();
    return inputs;
}

bool rtype::RenderSystem::isOpen() const
{
    return _window.isOpen();
}

void rtype::RenderSystem::onEntityAdded(const aecs::EntityPtr &entity)
{
    if (entity->hasComponent<rtype::SpriteComponent>() && entity->hasComponent<PositionComponent>())
        _entitiesMap[entity->getId()] = entity;
}

void rtype::RenderSystem::onEntityRemoved(const aecs::EntityPtr &entity)
{
    if (entity->hasComponent<rtype::SpriteComponent>() && entity->hasComponent<PositionComponent>())
        _entitiesMap.erase(entity->getId());
}

void rtype::RenderSystem::onEntityModified(const aecs::EntityPtr &entity)
{
    if (entity->hasComponent<rtype::SpriteComponent>() && entity->hasComponent<PositionComponent>())
        _entitiesMap[entity->getId()] = entity;
}
