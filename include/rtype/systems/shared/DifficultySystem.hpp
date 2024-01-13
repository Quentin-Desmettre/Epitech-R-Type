//
// Created by qdesmettre on 13/01/24.
//

#ifndef R_TYPE_DIFFICULTYSYSTEM_HPP
#define R_TYPE_DIFFICULTYSYSTEM_HPP

#include "aecs/SystemBase.hpp"

namespace rtype
{

    class DifficultySystem : public aecs::ALogicSystem
    {
      public:
        DifficultySystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities);
        ~DifficultySystem() override = default;

        aecs::EntityChanges update(aecs::UpdateParams &updateParams) override;

      private:
        aecs::EntityChanges updateDifficultyText();
        aecs::EntityChanges increaseDifficulty(aecs::UpdateParams &updateParams);
    };

} // namespace rtype

#endif // R_TYPE_DIFFICULTYSYSTEM_HPP
