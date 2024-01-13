//
// Created by edo on 26/12/23.
//

#ifndef R_TYPE_PLAYERONCONNECTIONSYSTEM_HPP
#define R_TYPE_PLAYERONCONNECTIONSYSTEM_HPP

#include "SystemBase.hpp"
#include "World.hpp"
#include <iostream>

class PlayerOnConnectionSystem : public aecs::ALogicSystem {
    public:
        PlayerOnConnectionSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities);

        ~PlayerOnConnectionSystem() override = default;

        std::vector<uint> _deletedEntities;

        aecs::EntityChanges update(aecs::UpdateParams &updateParams) override;

        void onEntityAdded(const aecs::EntityPtr &entity) override;

        void onEntityModified(const aecs::EntityPtr &entity) override;

        void onEntityRemoved(const aecs::EntityPtr &entity) override;
};

#endif // R_TYPE_PLAYERONCONNECTIONSYSTEM_HPP
