//
// Created by edo on 12/12/23.
//

#ifndef R_TYPE_INVULSYSTEM_HPP
#define R_TYPE_INVULSYSTEM_HPP

#include "aecs/SystemBase.hpp"
#include "aecs/World.hpp"
#include "rtype/components/DamageCollisionComponent.hpp"
#include "rtype/components/SpriteComponent.hpp"
namespace rtype
{

    class InvulSystem : public aecs::ALogicSystem
    {
      public:
        InvulSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities);
        ~InvulSystem() override = default;

        aecs::EntityChanges update(aecs::UpdateParams &updateParams) override;
    };

} // namespace rtype

#endif // R_TYPE_INVULSYSTEM_HPP
