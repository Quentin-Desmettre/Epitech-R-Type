//
// Created by edo on 19/12/23.
//

#ifndef R_TYPE_MONSTERDIE_HPP
#define R_TYPE_MONSTERDIE_HPP

#include "aecs/SystemBase.hpp"
#include "aecs/World.hpp"
#include "rtype/components/MonsterComponent.hpp"
#include "rtype/components/PositionComponent.hpp"
namespace rtype
{

    class MonsterDie : public aecs::ALogicSystem
    {
      public:
        MonsterDie(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities);
        ~MonsterDie() override = default;

        aecs::EntityChanges update(aecs::UpdateParams &updateParams) override;
    };

} // namespace rtype

#endif // R_TYPE_MONSTERDIE_HPP
