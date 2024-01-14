//
// Created by edo on 19/12/23.
//

#ifndef R_TYPE_MONSTERGENSYSTEM_HPP
#define R_TYPE_MONSTERGENSYSTEM_HPP

#include "aecs/SystemBase.hpp"
#include "aecs/World.hpp"
#include "rtype/components/PlayerComponent.hpp"
namespace rtype
{

    class MonsterGenSystem : public aecs::ALogicSystem
    {
      public:
        MonsterGenSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities);
        ~MonsterGenSystem() override = default;

        aecs::Entity &addRandomEnemy();

        aecs::EntityChanges update(aecs::UpdateParams &updateParams) override;

    private:
        aecs::EntityPtr _difficulty;
    };

} // namespace rtype

#endif // R_TYPE_MONSTERGENSYSTEM_HPP
