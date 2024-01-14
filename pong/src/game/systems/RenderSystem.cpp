//
// Created by qdesmettre on 05/12/23.
//

#include "RenderSystem.hpp"
#include "TextComponent.hpp"
#include "PositionComponent.hpp"
#include "TextComponent.hpp"
#include "PlayerComponent.hpp"
#include "BallComponent.hpp"

RenderSystem::RenderSystem(aecs::World &world,
                           const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
        ARenderSystem(world, entities, {}),
        _window(sf::VideoMode(1088, 640), "Pong")
{
}

void RenderSystem::onEntityAdded(const aecs::EntityPtr &entity)
{
    if (!entity->hasComponent<PositionComponent>())
        return;
    if (entity->hasComponent<SpriteComponent>() || entity->hasComponent<TextComponent>()) {
        addEntity(entity);
    }
}

void RenderSystem::onEntityRemoved(const aecs::EntityPtr &entity)
{
    if (entity->hasComponents({typeid(SpriteComponent), typeid(PositionComponent)}) ||
        entity->hasComponents({typeid(TextComponent), typeid(PositionComponent)})) {
        deleteEntity(entity->getId());
    }
}

void RenderSystem::onEntityModified(const aecs::EntityPtr &entity)
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

void RenderSystem::addEntity(const aecs::EntityPtr &entity)
{
    _entitiesToAdd.push_back(entity);
}

void RenderSystem::deleteEntity(std::size_t entityId)
{
    _entitiesToDelete.push_back(entityId);
}

void RenderSystem::_sortEntities()
{
    _sortedEntities.clear();
    for (auto &entity: _entitiesMap)
        _sortedEntities.push_back(entity.second);
    std::sort(_sortedEntities.begin(), _sortedEntities.end(), [](const aecs::EntityPtr &lhs, const aecs::EntityPtr &rhs) {
        int lhsZindex = lhs->hasComponent<TextComponent>() ? lhs->getComponent<TextComponent>().zIndex : lhs->getComponent<SpriteComponent>().zIndex;
        int rhsZindex = rhs->hasComponent<TextComponent>() ? rhs->getComponent<TextComponent>().zIndex : rhs->getComponent<SpriteComponent>().zIndex;
        return lhsZindex < rhsZindex;
    });
}

//sf::Rect<float> getRect(const std::shared_ptr<aecs::Entity> &entity)
//{
//    auto &sprite = entity->getComponent<SpriteComponent>();
//    auto &position = entity->getComponent<PositionComponent>();
//    sf::Rect rect = sprite.sprite.getGlobalBounds();
//
//    return rect;
//}

aecs::RenderInputs RenderSystem::render()
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
        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left)
                mouseInputs.emplace_back(aecs::MouseInputType::MOUSE_LEFT_CLICK_PRESSED, sf::Vector2f(sf::Mouse::getPosition(_window)));
            if (event.mouseButton.button == sf::Mouse::Right)
                mouseInputs.emplace_back(aecs::MouseInputType::MOUSE_RIGHT_CLICK_PRESSED, sf::Vector2f(sf::Mouse::getPosition(_window)));
        }
        if (event.type == sf::Event::MouseButtonReleased) {
            if (event.mouseButton.button == sf::Mouse::Left)
                mouseInputs.emplace_back(aecs::MouseInputType::MOUSE_LEFT_CLICK_RELEASED, sf::Vector2f(sf::Mouse::getPosition(_window)));
            if (event.mouseButton.button == sf::Mouse::Right)
                mouseInputs.emplace_back(aecs::MouseInputType::MOUSE_RIGHT_CLICK_RELEASED, sf::Vector2f(sf::Mouse::getPosition(_window)));
        }
        if (event.type == sf::Event::MouseMoved)
            mouseInputs.emplace_back(aecs::MouseInputType::MOUSE_MOVE, sf::Vector2f(sf::Mouse::getPosition(_window)));
    }
    for (int i = 0; i < sf::Keyboard::KeyCount; i++) {
        if (sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(i))) {
            inputs.push_back(static_cast<aecs::RenderInput>(i));
        }
    }

    // Render
    _window.clear(sf::Color(63, 63, 63));
    _flushBuffers();
    for (auto &entity: _sortedEntities) {
        if (entity->hasComponent<SpriteComponent>())
            drawSprite(entity);
        if (entity->hasComponent<TextComponent>())
            drawText(entity);

//        if (entity->hasComponent<PlayerComponent>() || entity->hasComponent<BallComponent>()) {
//
//            sf::Rect rect = getRect(entity);
//            sf::RectangleShape rectangle(sf::Vector2f(rect.width, rect.height));
//            rectangle.setPosition(rect.left, rect.top);
//            rectangle.setFillColor(sf::Color(255, 0, 0, 120));
//            _window.draw(rectangle);
//        }
    }
    _window.display();
    return {inputs, mouseInputs};
}

void RenderSystem::drawText(const aecs::EntityPtr &entity)
{
    auto &text = entity->getComponent<TextComponent>();
    auto &pos = entity->getComponent<PositionComponent>();
    text._text.setPosition(pos.x, pos.y);
    _window.draw(text._text);
}

void RenderSystem::drawSprite(const aecs::EntityPtr &entity)
{
    auto &sprite = entity->getComponent<SpriteComponent>();
    auto &pos = entity->getComponent<PositionComponent>();
    sprite.sprite.setPosition(pos.x, pos.y);
    _window.draw(sprite.sprite);
}

void RenderSystem::close()
{
    _window.close();
}

bool RenderSystem::isOpen() const
{
    return _window.isOpen();
}

void RenderSystem::_flushBuffers()
{
    // Add entities
    for (auto &entity: _entitiesToAdd)
        _entitiesMap[entity->getId()] = entity;

    // Delete entities
    for (auto &id: _entitiesToDelete) {
        _entitiesMap.erase(id);
    }

    // Sort entities
    if (!_entitiesToAdd.empty() || !_entitiesToDelete.empty())
        _sortEntities();

    // Clear buffers
    _entitiesToDelete.clear();
    _entitiesToAdd.clear();
}
