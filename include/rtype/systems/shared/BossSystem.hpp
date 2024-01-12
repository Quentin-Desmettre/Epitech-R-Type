//
// Created by edo on 11/01/24.
//

#ifndef R_TYPE_BOSSSYSTEM_HPP
#define R_TYPE_BOSSSYSTEM_HPP

#include "aecs/SystemBase.hpp"
#include "aecs/World.hpp"
#include "rtype/components/BossComponent.hpp"
#include "rtype/components/PlayerComponent.hpp"
#include "rtype/components/PositionComponent.hpp"
#include "rtype/components/VelocityComponent.hpp"

#include <iostream>
namespace rtype
{
    class BossSystem : public aecs::ALogicSystem
    {
      public:
        BossSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities);
        ~BossSystem() override = default;

        aecs::EntityChanges update(aecs::UpdateParams &updateParams) override;
        void onEntityRemoved(const aecs::EntityPtr &entity) override;
        void onEntityModified(const aecs::EntityPtr &entity) override;

        std::vector<aecs::EntityPtr> players;
    };

} // namespace rtype

#endif // R_TYPE_BOSSSYSTEM_HPP
