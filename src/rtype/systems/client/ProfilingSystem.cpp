//
// Created by qdesmettre on 06/01/24.
//

#include "rtype/systems/client/ProfilingSystem.hpp"

#include "aecs/World.hpp"
#include "rtype/components/PositionComponent.hpp"
#include "rtype/components/TextComponent.hpp"
#include <utility>

rtype::ProfilingSystem::ProfilingSystem(aecs::World &world,
                                        const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities,
                                        float timeBetweenUpdate) :
    ALogicSystem(world, entities, {}),
    _timeBetweenUpdate(timeBetweenUpdate)
{
}

rtype::ProfilingSystem &rtype::ProfilingSystem::addProfiling(rtype::ProfilingSystem::ProfilingFunction function,
                                                             const std::string &name)
{
    // When we add a profiling functions, we need to create an entity that will hold the result of the function
    // This entity is a text that will be displayed on the screen
    auto &entity = _world.createEntity();
    entity.addComponent<TextComponent>(name, 20, sf::Color::White, false);
    entity.addComponent<PositionComponent>(20, _profilers.size() * 30 + 20);
    _profilers[entity.getId()] = {name, function};
    return *this;
}

rtype::ProfilingSystem &rtype::ProfilingSystem::clear()
{
    _profilers.clear();
    return *this;
}

aecs::EntityChanges rtype::ProfilingSystem::update(aecs::UpdateParams &updateParams)
{
    _elapsedTime += updateParams.deltaTime;
//    if (_elapsedTime < _timeBetweenUpdate)
//        return {};
    _elapsedTime = 0;

    // We iterate over all the entities that are registered in the system
    // We then call the profiling function associated with the entity
    // The result of the function is then set as the text of the entity
    for (auto &[id, function] : _profilers) {
        auto &[name, profiler] = function;
        auto entity = _world.getEntity(id);
        if (!entity)
            continue;
        auto &text = entity->getComponent<TextComponent>();
        text._text.setString(name + ": " + profiler(_world));
    }
    return {};
}
