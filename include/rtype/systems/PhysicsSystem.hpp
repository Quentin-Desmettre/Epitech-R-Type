//
// Created by qdesmettre on 05/12/23.
//

#ifndef R_TYPE_PHYSICSSYSTEM_HPP
#define R_TYPE_PHYSICSSYSTEM_HPP

#include "aecs/SystemBase.hpp"
#include "aecs/World.hpp"
#include "rtype/components/PositionComponent.hpp"
#include "rtype/components/VelocityComponent.hpp"
#include <iostream>

namespace rtype {

    class PhysicsSystem: public aecs::ILogicSystem {
    public:
        PhysicsSystem(aecs::World &world,
                      const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
                _world(world)
        {
            for (auto &[_id, entity] : entities)
                PhysicsSystem::onEntityAdded(entity);
        }
        ~PhysicsSystem() override = default;

        void update(const std::vector<aecs::IRenderSystem::RenderInput> &inputs) override {
            for (auto &[_id, entity] : _entitiesMap) {
                auto &position = entity->getComponent<PositionComponent>();
                auto &velocity = entity->getComponent<VelocityComponent>();

                position.x += velocity.x;
                position.y += velocity.y;

                if (position.x > 600 || position.x < 0)
                    velocity.x = -velocity.x;
                if (position.y > 400 || position.y < 0)
                    velocity.y = -velocity.y;
            }
        }

        void onEntityAdded(const aecs::EntityPtr &entity) override {
            if (entity->hasComponent<PositionComponent>() && entity->hasComponent<VelocityComponent>()) {
                _entitiesMap[entity->getId()] = entity;
            }
        }

        void onEntityRemoved(const aecs::EntityPtr &entity) override {
            if (!entity->hasComponent<PositionComponent>() || !entity->hasComponent<VelocityComponent>())
                _entitiesMap.erase(entity->getId());
        }

        void onEntityModified(const aecs::EntityPtr &entity) override {
            if (entity->hasComponent<PositionComponent>() && entity->hasComponent<VelocityComponent>())
                _entitiesMap[entity->getId()] = entity;
        }

    private:
        aecs::World &_world;
        std::map<std::size_t, aecs::EntityPtr> _entitiesMap;

    };

} // rtype

#endif //R_TYPE_PHYSICSSYSTEM_HPP
