//
// Created by edo on 19/12/23.
//

#ifndef R_TYPE_MONSTERBULLET_HPP
#define R_TYPE_MONSTERBULLET_HPP

#include "aecs/SystemBase.hpp"
#include "aecs/World.hpp"
#include "rtype/components/MonsterComponent.hpp"
#include "rtype/components/PositionComponent.hpp"
namespace rtype
{

    class MonsterBullet : public aecs::ALogicSystem
    {
      public:
        MonsterBullet(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities);
        ~MonsterBullet() override = default;

        aecs::EntityChanges update(aecs::UpdateParams &updateParams) override;
    };

} // namespace rtype

#endif // R_TYPE_MONSTERBULLET_HPP
