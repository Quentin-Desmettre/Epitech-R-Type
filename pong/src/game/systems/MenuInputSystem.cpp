//
// Created by edo on 06/12/23.
//

#include "MenuInputSystem.hpp"

namespace aecs
{
    MenuInputSystem::MenuInputSystem(aecs::World &world,
                                     const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
            ALogicSystem(world, entities, {typeid(InputComponent)})
    {
    }

    aecs::EntityChanges MenuInputSystem::update(aecs::UpdateParams &updateParams)
    {
        aecs::EntityChanges changes;
        for (auto &[_id, entity] : _entitiesMap) {
            const auto &inputPressed = entity->getComponent<InputComponent>();

            for (auto &[_, clientInputs] : updateParams.inputs) {
                for (auto &input : clientInputs) {
                    if (input == inputPressed.input) {
                        inputPressed.onInput();
                        return changes;
                    }
                }
            }
        }
        return changes;
    }
} // namespace aecs
