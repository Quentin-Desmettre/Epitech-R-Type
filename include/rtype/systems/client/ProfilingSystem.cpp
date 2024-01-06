//
// Created by qdesmettre on 06/01/24.
//

#include "ProfilingSystem.hpp"
#include "rtype/components/TextComponent.hpp"

rtype::ProfilingSystem::ProfilingSystem(aecs::World &world,
                                        const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities):
    ALogicSystem(world, entities, {})
{
}

void rtype::ProfilingSystem::addProfiling(const rtype::ProfilingSystem::ProfilingFunction &function)
{
    // When we add a profiling functions, we need to create an entity that will hold the result of the function
    // This entity is a text that will be displayed on the screen
    auto entity = _world.createEntity();
    entity->addComponent<TextComponent>();
}
