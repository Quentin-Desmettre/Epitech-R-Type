//
// Created by qdesmettre on 05/12/23.
//

#include "rtype/systems/client/RenderSystem.hpp"
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
    if (entity->hasComponents(_componentsNeeded)) {
        addEntity(entity);
    }
}

void rtype::RenderSystem::onEntityRemoved(const aecs::EntityPtr &entity)
{
    if (entity->hasComponents(_componentsNeeded)) {
        deleteEntity(entity->getId());
    }
}

void rtype::RenderSystem::onEntityModified(const aecs::EntityPtr &entity)
{
    bool hasComponents = entity->hasComponents(_componentsNeeded);
    bool isInMap = _entitiesMap.find(entity->getId()) != _entitiesMap.end();

    if (hasComponents && !isInMap) {
        addEntity(entity);
    } else if (!hasComponents && isInMap) {
        deleteEntity(entity->getId());
    }
}

void rtype::RenderSystem::addEntity(const aecs::EntityPtr &entity)
{
    _entitiesToAdd.push_back(entity);
}

void rtype::RenderSystem::deleteEntity(std::size_t entityId)
{
    _entitiesToDelete.push_back(entityId);
}

void rtype::RenderSystem::_sortEntities()
{
    _sortedEntities.clear();
    for (auto &entity : _entitiesMap)
        _sortedEntities.push_back(entity.second);
    std::sort(_sortedEntities.begin(), _sortedEntities.end(), [](const aecs::EntityPtr &lhs, const aecs::EntityPtr &rhs) {
        return lhs->getComponent<rtype::SpriteComponent>().zIndex < rhs->getComponent<rtype::SpriteComponent>().zIndex;
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
    _flushBuffers();
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
    _flushBuffers();
    return inputs;
}

bool rtype::RenderSystem::isOpen() const
{
    return _window.isOpen();
}

void rtype::RenderSystem::_flushBuffers()
{
    // Add entities
    for (auto &entity : _entitiesToAdd)
        _entitiesMap[entity->getId()] = entity;

    // Delete entities
    for (auto &id : _entitiesToDelete) {
        _entitiesMap.erase(id);
    }

    // Clear buffers
    _entitiesToDelete.clear();
    _entitiesToAdd.clear();

    // Sort entities
    _sortEntities();
}
