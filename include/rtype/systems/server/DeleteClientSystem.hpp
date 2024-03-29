//
// Created by qdesmettre on 20/12/23.
//

#ifndef R_TYPE_DELETECLIENTSYSTEM_HPP
#define R_TYPE_DELETECLIENTSYSTEM_HPP

#include "aecs/SystemBase.hpp"
#include "aecs/World.hpp"

namespace rtype
{
    class DeleteClientSystem : public aecs::ALogicSystem
    {
      public:
        DeleteClientSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities);

        ~DeleteClientSystem() override = default;

        aecs::EntityChanges update(aecs::UpdateParams &updateParams) override;
    };
} // namespace rtype

#endif // R_TYPE_DELETECLIENTSYSTEM_HPP
