//
// Created by qdesmettre on 04/01/24.
//

#ifndef R_TYPE_MAPSYSTEM_HPP
#define R_TYPE_MAPSYSTEM_HPP

#include "aecs/SystemBase.hpp"

namespace rtype {

    /**
     * @brief System that handle the map
     *
     * its role is to randomly create the blocks that compose the map
     */
    class MapSystem: public aecs::ALogicSystem
    {
    public:
        MapSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities);
        ~MapSystem() override = default;

        aecs::EntityChanges update(aecs::UpdateParams &updateParams) override;

    private:
    };
}


#endif //R_TYPE_MAPSYSTEM_HPP
