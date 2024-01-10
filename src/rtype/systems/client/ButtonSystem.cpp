//
// Created by qdesmettre on 20/12/23.
//

#include "rtype/systems/client/ButtonSystem.hpp"
#include "rtype/components/PositionComponent.hpp"
#include "rtype/components/TextComponent.hpp"
#include "aecs/Entity.hpp"
#include <iostream>

namespace rtype
{

    ButtonSystem::ButtonSystem(
        aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
        ALogicSystem(world, entities, {typeid(TextComponent), typeid(PositionComponent)})
    {
    }

    aecs::EntityChanges ButtonSystem::update(aecs::UpdateParams &updateParams)
    {
        aecs::EntityChanges changes;
        if (updateParams.mouseInputs.empty())
            return {};

        for (auto &[_, entity] : _entitiesMap) {
            auto &component = entity->getComponent<TextComponent>();
            const auto &text = component._text.getGlobalBounds();

            for (auto &input : updateParams.mouseInputs) {
                sf::Vector2f mousePos = input.second;
                if (text.contains(mousePos) && component.onClick) {
                    component.onOver();
                    if (input.first == aecs::MOUSE_LEFT_CLICK_PRESSED) {
                        component.onPress();
                    }
                    if (input.first == aecs::MOUSE_LEFT_CLICK_RELEASED) {
                        component.onClick();
                        return changes;
                    }
                } else {
                    component.removeOver();
                }
            }
        }
        return changes;
    }

} // namespace rtype
