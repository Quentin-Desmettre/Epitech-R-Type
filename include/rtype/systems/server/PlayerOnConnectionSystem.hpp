//
// Created by edo on 26/12/23.
//

#ifndef R_TYPE_PLAYERONCONNECTIONSYSTEM_HPP
#define R_TYPE_PLAYERONCONNECTIONSYSTEM_HPP

#include "aecs/SystemBase.hpp"
#include "aecs/World.hpp"
#include "rtype/components/ParallaxComponent.hpp"
#include "rtype/components/PositionComponent.hpp"
#include "rtype/components/SpriteComponent.hpp"
#include "rtype/components/VelocityComponent.hpp"
#include <iostream>

namespace rtype
{

    class PlayerOnConnectionSystem : public aecs::ALogicSystem
    {
      public:
        PlayerOnConnectionSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
            ALogicSystem(world, entities,
                         {typeid(ClientAdressComponent)})
        {
        }
        ~PlayerOnConnectionSystem() override = default;

        aecs::EntityChanges update(aecs::UpdateParams &updateParams) override
        {
            aecs::EntityChanges changes;
            return {};
        }

        void onEntityAdded(const aecs::EntityPtr &entity) override
        {
            if (entity->hasComponents(_componentsNeeded)) {
                _entitiesMap[entity->getId()] = entity;
            }
        }

        void onEntityModified(const aecs::EntityPtr &entity) override
        {
            if (entity->hasComponents(_componentsNeeded)) {
                if (_entitiesMap.find(entity->getId()) == _entitiesMap.end()) {
                    _entitiesMap[entity->getId()] = entity;
                    entity->addComponent<PlayerComponent>();
                    EntityFactory::toPlayer(*entity);
                }
            } else
                _entitiesMap.erase(entity->getId());
        }
    };

} // namespace rtype

#endif // R_TYPE_PLAYERONCONNECTIONSYSTEM_HPP
