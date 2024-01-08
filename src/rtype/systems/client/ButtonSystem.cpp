//
// Created by qdesmettre on 20/12/23.
//

#include "rtype/systems/client/ButtonSystem.hpp"
#include "rtype/components/PositionComponent.hpp"
#include "rtype/components/TextComponent.hpp"

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
            const auto &component = entity->getComponent<TextComponent>();
            const auto &text = component._text.getGlobalBounds();

            for (auto &input : updateParams.mouseInputs) {
                if (input.first == aecs::MOUSE_LEFT_CLICK) {
                    sf::Vector2f mousePos = input.second;
                    if (text.contains(mousePos) && component.onClick) {
                        component.onClick();
                        return changes;
                    }
                }
            }
        }
        return changes;
    }

} // namespace rtype
