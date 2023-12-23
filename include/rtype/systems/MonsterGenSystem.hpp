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
        MonsterGenSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
            ALogicSystem(world, entities, {typeid(PlayerComponent)})
        {
        }
        ~MonsterGenSystem() override = default;

        static void addRandomEnemy();

        void update(const aecs::UpdateParams &updateParams) override;
    };

} // namespace rtype

#endif // R_TYPE_MONSTERGENSYSTEM_HPP
