//
// Created by edo on 03/01/24.
//

#ifndef R_TYPE_NODEMONSTERSYSTEM_HPP
#define R_TYPE_NODEMONSTERSYSTEM_HPP

#include "aecs/SystemBase.hpp"
#include "aecs/World.hpp"
namespace rtype
{

    class NodeMonsterSystem : public aecs::ALogicSystem
    {
      public:
        NodeMonsterSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities);
        ~NodeMonsterSystem() override = default;

        std::vector<sf::Vector2f> positions;

        aecs::EntityChanges update(aecs::UpdateParams &updateParams) override;
        void follow(aecs::EntityPtr &entity1, float delta);
        void wave(aecs::Entity &entity, float delta);

        void onEntityRemoved(const aecs::EntityPtr &entity) override;
        std::vector<float> _dead;
        std::vector<float> _time;
    };

} // namespace rtype

#endif // R_TYPE_NODEMONSTERSYSTEM_HPP
