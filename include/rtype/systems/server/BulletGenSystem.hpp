//
// Created by edo on 09/01/24.
//

#ifndef R_TYPE_BULLETGENSYSTEM_HPP
#define R_TYPE_BULLETGENSYSTEM_HPP

#include "aecs/SystemBase.hpp"
#include "aecs/World.hpp"

namespace rtype
{
    class BulletGenSystem : public aecs::ALogicSystem
    {
      public:
        BulletGenSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities);

        ~BulletGenSystem() override = default;

        aecs::EntityChanges update(aecs::UpdateParams &updateParams) override;
    };
} // namespace rtype

#endif // R_TYPE_BULLETGENSYSTEM_HPP
