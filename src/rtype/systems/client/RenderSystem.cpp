//
// Created by qdesmettre on 05/12/23.
//

#include "rtype/systems/client/RenderSystem.hpp"
#include "rtype/components/TextComponent.hpp"

rtype::RenderSystem::RenderSystem(aecs::World &world,
                                  const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
    ARenderSystem(world, entities, {}),
    _window(sf::VideoMode(1088, 640), "R-Type")
{
}

void rtype::RenderSystem::onEntityAdded(const aecs::EntityPtr &entity)
{
    if (!entity->hasComponent<PositionComponent>())
        return;
    if (entity->hasComponent<SpriteComponent>() || entity->hasComponent<TextComponent>()) {
        addEntity(entity);
    }
}

void rtype::RenderSystem::onEntityRemoved(const aecs::EntityPtr &entity)
{
    if (entity->hasComponents({typeid(SpriteComponent), typeid(PositionComponent)}) ||
        entity->hasComponents({typeid(TextComponent), typeid(PositionComponent)})) {
        deleteEntity(entity->getId());
    }
}

void rtype::RenderSystem::onEntityModified(const aecs::EntityPtr &entity)
{
    bool isSprite = entity->hasComponents({typeid(SpriteComponent), typeid(PositionComponent)});
    bool isText = entity->hasComponents({typeid(TextComponent), typeid(PositionComponent)});
    bool isInMap = _entitiesMap.find(entity->getId()) != _entitiesMap.end();

    if ((isSprite || isText) && !isInMap) {
        addEntity(entity);
    } else if (!isSprite && !isText && isInMap) {
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
    std::sort(_sortedEntities.begin(), _sortedEntities.end(),
              [](const aecs::EntityPtr &lhs, const aecs::EntityPtr &rhs) {
                  auto lIndex = lhs->hasComponent<rtype::SpriteComponent>()
                                    ? lhs->getComponent<rtype::SpriteComponent>().zIndex
                                    : lhs->getComponent<rtype::TextComponent>().zIndex;
                  auto rIndex = rhs->hasComponent<rtype::SpriteComponent>()
                                    ? rhs->getComponent<rtype::SpriteComponent>().zIndex
                                    : rhs->getComponent<rtype::TextComponent>().zIndex;
                  return lIndex < rIndex;
              });
}

aecs::RenderInputs rtype::RenderSystem::render()
{
    aecs::ClientInputs inputs;
    aecs::MouseInputs mouseInputs;

    // Poll events
    sf::Event event{};
    while (_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            _window.close();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            _window.close();
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            mouseInputs.emplace_back(1, sf::Vector2f(sf::Mouse::getPosition(_window)));
        if (event.type == sf::Event::MouseMoved)
            mouseInputs.emplace_back(0, sf::Vector2f(sf::Mouse::getPosition(_window)));
    }
    for (int i = 0; i < sf::Keyboard::KeyCount; i++) {
        if (sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(i)))
            inputs.push_back(static_cast<aecs::RenderInput>(i));
    }

    // Render
    _window.clear(sf::Color(63, 63, 63));
    _flushBuffers();
    for (auto &entity : _sortedEntities) {
        if (entity->hasComponent<SpriteComponent>())
            drawSprite(entity);
        if (entity->hasComponent<TextComponent>())
            drawText(entity);
    }
    _window.display();
    _flushBuffers();
    return {inputs, mouseInputs};
}

void rtype::RenderSystem::drawText(const aecs::EntityPtr &entity)
{
    auto &text = entity->getComponent<rtype::TextComponent>();
    auto &pos = entity->getComponent<PositionComponent>();
    text._text.setPosition(pos.x, pos.y);
    _window.draw(text._text);
}

void rtype::RenderSystem::drawSprite(const aecs::EntityPtr &entity)
{
    auto &sprite = entity->getComponent<rtype::SpriteComponent>();
    auto &pos = entity->getComponent<PositionComponent>();
    sprite.sprite.setPosition(pos.x, pos.y);
    if (entity->hasComponent<ShaderComponent>()) {
        auto &shader = entity->getComponent<ShaderComponent>();
        _window.draw(sprite.sprite, shader.shader.get());
    } else
        _window.draw(sprite.sprite);
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

    // Sort entities
    if (!_entitiesToAdd.empty() || !_entitiesToDelete.empty())
        _sortEntities();

    // Clear buffers
    _entitiesToDelete.clear();
    _entitiesToAdd.clear();
}
