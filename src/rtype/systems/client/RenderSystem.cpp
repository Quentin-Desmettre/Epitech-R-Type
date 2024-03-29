//
// Created by qdesmettre on 05/12/23.
//

#include "rtype/systems/client/RenderSystem.hpp"
#include "rtype/components/CollidableComponent.hpp"
#include "rtype/components/DrawHealthBar.hpp"
#include "rtype/components/HPComponent.hpp"
#include "rtype/components/TextComponent.hpp"
#include "rtype/components/XPComponent.hpp"
#include "shared/SFMLLoader.hpp"

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
        deleteEntity(entity);
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
        deleteEntity(entity);
    }
}

void rtype::RenderSystem::addEntity(const aecs::EntityPtr &entity)
{
    _entitiesToAdd.push_back(entity);
}

void rtype::RenderSystem::deleteEntity(const aecs::EntityPtr &entity)
{
    _entitiesToDelete.push_back(entity);
}

void rtype::RenderSystem::_sortEntities()
{
    _sortedEntities.clear();
    for (auto &entity: _entitiesMap)
        _sortedEntities.push_back(entity.second);
    std::sort(_sortedEntities.begin(), _sortedEntities.end(),
              [](const aecs::EntityPtr &lhs, const aecs::EntityPtr &rhs) {
                  int lhsZindex = lhs->hasComponent<rtype::TextComponent>()
                                      ? lhs->getComponent<rtype::TextComponent>().zIndex
                                      : lhs->getComponent<rtype::SpriteComponent>().zIndex;
                  int rhsZindex = rhs->hasComponent<rtype::TextComponent>()
                                      ? rhs->getComponent<rtype::TextComponent>().zIndex
                                      : rhs->getComponent<rtype::SpriteComponent>().zIndex;
                  return lhsZindex < rhsZindex;
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
        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left)
                mouseInputs.emplace_back(aecs::MouseInputType::MOUSE_LEFT_CLICK_PRESSED,
                                         sf::Vector2f(sf::Mouse::getPosition(_window)));
            if (event.mouseButton.button == sf::Mouse::Right)
                mouseInputs.emplace_back(aecs::MouseInputType::MOUSE_RIGHT_CLICK_PRESSED,
                                         sf::Vector2f(sf::Mouse::getPosition(_window)));
        }
        if (event.type == sf::Event::MouseButtonReleased) {
            if (event.mouseButton.button == sf::Mouse::Left)
                mouseInputs.emplace_back(aecs::MouseInputType::MOUSE_LEFT_CLICK_RELEASED,
                                         sf::Vector2f(sf::Mouse::getPosition(_window)));
            if (event.mouseButton.button == sf::Mouse::Right)
                mouseInputs.emplace_back(aecs::MouseInputType::MOUSE_RIGHT_CLICK_RELEASED,
                                         sf::Vector2f(sf::Mouse::getPosition(_window)));
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
    for (auto &entity : _sortedEntities) {
        if (entity->hasComponent<SpriteComponent>())
            drawSprite(entity);
        if (entity->hasComponent<TextComponent>())
            drawText(entity);
        if (entity->hasComponent<DrawHealthBar>()) {
            drawHealthBar(entity);
            if (entity->hasComponent<XPComponent>())
                drawXPBar(entity);
        }
    }
    _window.display();
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

void rtype::RenderSystem::drawHealthBar(const aecs::EntityPtr &entity)
{
    auto &hp = entity->getComponent<HPComponent>();
    auto &sprite = entity->getComponent<SpriteComponent>();
    auto &pos = entity->getComponent<PositionComponent>();
    float maxSize = sprite._size.x * 0.8f;
    auto origin = sprite.sprite.getOrigin();
    sf::Vector2f rectsPos = {pos.x - origin.x - (sprite._size.x - maxSize), pos.y - origin.y - 40};
    if (hp.hp <= 0)
        return;
    drawBar(maxSize, hp.hp / hp.maxHp, sf::Color::Red, rectsPos);
}

void rtype::RenderSystem::drawXPBar(const aecs::EntityPtr &entity)
{
    auto &xp = entity->getComponent<XPComponent>();
    auto &sprite = entity->getComponent<SpriteComponent>();
    auto &pos = entity->getComponent<PositionComponent>();
    float maxSize = sprite._size.x * 0.7f;
    auto origin = sprite.sprite.getOrigin();
    sf::Vector2f rectsPos = {pos.x - origin.x - (sprite._size.x - maxSize) + sprite._size.x * 0.2f, pos.y - origin.y - 60};
    drawBar(maxSize, static_cast<float>(xp.getXp()) / static_cast<float>(xp.getXpForLevelup()), sf::Color::Green, rectsPos);
    sf::Text text;
    text.setString(std::to_string(xp.getLevel()));
    sf::Font *f = SFMLLoader::loadFont("assets/fonts/Minecraft.ttf");
    if (f)
        text.setFont(*f);
    text.setCharacterSize(16);
    text.setFillColor(sf::Color::White);
    text.setPosition({rectsPos.x - sprite._size.x * 0.1f, rectsPos.y - 5});
    _window.draw(text);
}

void rtype::RenderSystem::drawBar(float width, float percentage, sf::Color color, sf::Vector2f position)
{
    sf::RectangleShape filled({width * percentage, 10});
    sf::RectangleShape empty({width, 10});
    sf::Color emptyColor = {
            static_cast<sf::Uint8>(color.r / 2),
            static_cast<sf::Uint8>(color.g / 2),
            static_cast<sf::Uint8>(color.b / 2),
            static_cast<sf::Uint8>(color.a / 2)
    };

    filled.setPosition(position);
    empty.setPosition(position);
    empty.setFillColor(emptyColor);
    filled.setFillColor(color);
    _window.draw(empty);
    _window.draw(filled);
}

void rtype::RenderSystem::close()
{
    _window.close();
}

bool rtype::RenderSystem::isOpen() const
{
    return _window.isOpen();
}

void rtype::RenderSystem::_flushBuffers()
{
    // Add entities
    for (auto &entity: _entitiesToAdd)
        _entitiesMap[entity->getId()] = entity;

    // Delete entities
    for (auto &entity: _entitiesToDelete) {
        _entitiesMap.erase(entity->getId());
    }

    // Sort entities
    if (!_entitiesToAdd.empty() || !_entitiesToDelete.empty())
        _sortEntities();

    // Clear buffers
    _entitiesToDelete.clear();
    _entitiesToAdd.clear();
}
