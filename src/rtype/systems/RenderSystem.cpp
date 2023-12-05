//
// Created by qdesmettre on 05/12/23.
//

#include "rtype/systems/RenderSystem.hpp"
#include "rtype/components/PositionComponent.hpp"
#include "rtype/components/SpriteComponent.hpp"

rtype::RenderSystem::RenderSystem(aecs::World &world,
                                  const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
    ARenderSystem(world, entities, {typeid(rtype::SpriteComponent), typeid(rtype::PositionComponent)}),
    _window(sf::VideoMode(800, 600), "R-Type")
{
}

std::vector<aecs::RenderInput> rtype::RenderSystem::render()
{
    std::vector<aecs::RenderInput> inputs;

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
