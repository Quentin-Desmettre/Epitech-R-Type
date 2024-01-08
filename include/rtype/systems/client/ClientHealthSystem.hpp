//
// Created by Clément Vandeville on 08/01/2024.
//

#ifndef R_TYPE_CLIENTHEALTHSYSTEM_HPP
#define R_TYPE_CLIENTHEALTHSYSTEM_HPP

#include "aecs/SystemBase.hpp"
#include "aecs/World.hpp"

namespace rtype {
    class ClientHealthSystem : public aecs::ALogicSystem {
        public:
            ClientHealthSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities);

            ~ClientHealthSystem() override = default;

            aecs::EntityChanges update(aecs::UpdateParams &updateParams) override;
    };
}

#endif //R_TYPE_CLIENTHEALTHSYSTEM_HPP
