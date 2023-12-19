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
        MonsterBullet(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
            ALogicSystem(world, entities, {typeid(MonsterComponent), typeid(PositionComponent)})
        {
        }
        ~MonsterBullet() override = default;

        void update(const std::vector<aecs::RenderInput> &inputs, float deltaTime) override
        {
            for (auto &[_id, entity] : _entitiesMap) {
                auto &monster = entity->getComponent<MonsterComponent>();
                auto &position = entity->getComponent<PositionComponent>();
                if (monster._lil)
                    continue;
                monster.timeSinceLastShoot += deltaTime;

                if (monster.timeSinceLastShoot > 15) {
                    monster.timeSinceLastShoot = 0;
                    EntityFactory::createBullet(sf::Vector2f(position.x - 10, position.y + 10), sf::Vector2f(-50, 0), 1, false);
                }
            }
        }
    };

} // namespace rtype

#endif // R_TYPE_MONSTERBULLET_HPP
