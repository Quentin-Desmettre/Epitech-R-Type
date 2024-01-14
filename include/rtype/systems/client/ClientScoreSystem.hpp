//
// Created by Clément Vandeville on 03/01/2024.
//

#ifndef R_TYPE_CLIENTSCORESYSTEM_HPP
#define R_TYPE_CLIENTSCORESYSTEM_HPP


#include "aecs/SystemBase.hpp"
#include "rtype/components/MonsterComponent.hpp"
#include "aecs/World.hpp"
#include <iostream>

namespace rtype {
    class ClientScoreSystem : public aecs::ALogicSystem {
        public:
            ClientScoreSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities);

            ~ClientScoreSystem() override = default;

            aecs::EntityChanges update(aecs::UpdateParams &updateParams) override;
            void onEntityRemoved(const aecs::EntityPtr &entity) override;

        private:
            int _score = 0;
    };
}


#endif //R_TYPE_CLIENTSCORESYSTEM_HPP
