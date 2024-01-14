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

        void onEntityAdded(const aecs::EntityPtr &entity) override;
        void onEntityRemoved(const aecs::EntityPtr &entity) override;
        void onEntityModified(const aecs::EntityPtr &entity) override;

      private:
        static constexpr const float DIFFICULTY_INCREASE_LIL_MONSTER = 0.01;
        static constexpr const float DIFFICULTY_INCREASE_BIG_MONSTER = 0.05;
        static constexpr const float DIFFICULTY_INCREASE_BOSS = 1;

        aecs::EntityChanges updateDifficultyText(aecs::EntityChanges changes);
        aecs::EntityChanges increaseDifficulty(aecs::UpdateParams &updateParams, aecs::EntityChanges changes);

        aecs::EntityPtr _difficulty;
        std::set<aecs::EntityPtr> _nodes;
    };

} // namespace rtype

#endif // R_TYPE_DIFFICULTYSYSTEM_HPP
