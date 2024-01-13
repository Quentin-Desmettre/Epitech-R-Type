//
// Created by edo on 12/12/23.
//

#include "rtype/systems/shared/DamageCollisionSystem.hpp"
#include "rtype/components/InvulComponent.hpp"

namespace rtype
{
    DamageCollisionSystem::DamageCollisionSystem(aecs::World &world,
                                                 const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
        ALogicSystem(
            world, entities,
            {typeid(SpriteComponent), typeid(DamageCollisionComponent), typeid(HPComponent), typeid(PositionComponent)})
    {
    }

    void DamageCollisionSystem::onEntityAdded(const aecs::EntityPtr &entity)
    {
        auto oldSize = _entitiesMap.size();
        ALogicSystem::onEntityAdded(entity);

        if (_entitiesMap.size() != oldSize)
            entities.push_back(entity);
    }

    void DamageCollisionSystem::onEntityRemoved(const aecs::EntityPtr &entity)
    {
        auto oldSize = _entitiesMap.size();
        ALogicSystem::onEntityRemoved(entity);

        if (_entitiesMap.size() != oldSize) {
            std::erase(entities, entity);
        }
    }

    void DamageCollisionSystem::onEntityModified(const aecs::EntityPtr &entity)
    {
        auto oldSize = _entitiesMap.size();
        ALogicSystem::onEntityModified(entity);

        if (_entitiesMap.size() < oldSize) {
            std::erase(entities, entity);
        } else if (_entitiesMap.size() > oldSize) {
            entities.push_back(entity);
        }
    }

    sf::Rect<float> DamageCollisionSystem::getRect(aecs::Entity *entity, const PositionComponent &position)
    {
        auto &sprite = entity->getComponent<SpriteComponent>();
        sprite.sprite.setPosition(position.x, position.y);
        sf::Rect rect = sprite.sprite.getGlobalBounds();
        float reduction = 0.3;
        rect.width *= 1 - reduction;
        rect.height *= 1 - reduction;
        rect.top += rect.height * (reduction / 2.0f);
        rect.left += rect.width * (reduction / 2.0f);
        return rect;
    }

    bool DamageCollisionSystem::killed(const std::shared_ptr<aecs::Entity> &entity,
                                       const std::shared_ptr<aecs::Entity> &entity2)
    {
        auto &damage = entity->getComponent<DamageCollisionComponent>();
        auto &damage2 = entity2->getComponent<DamageCollisionComponent>();
        if (damage.team == damage2.team)
            return false;
        if (damage.type == DamageCollisionComponent::ObjectType::LG_BULLET &&
            damage2.type == DamageCollisionComponent::ObjectType::LG_BULLET)
            return true;
        return false;
    }

    void DamageCollisionSystem::addPowerUp(aecs::EntityChanges &changes)
    {
        if (!_world.getIsServer())
            return;
        for (auto &id : changes.deletedEntities) {
            auto entity = _entitiesMap.find(id);
            if (entity == _entitiesMap.end() || !entity->second->hasComponent<MonsterComponent>())
                continue;
            int random = rand() % 100;
            int dropChance = entity->second->getComponent<MonsterComponent>()._lil ? 15 : 30;
            if (random < dropChance) {
                auto &position = entity->second->getComponent<PositionComponent>();
                aecs::Entity &power = EntityFactory::createPower(sf::Vector2f(position.x, position.y), PowerComponent::getRandomPower());
                changes.editedEntities.insert(power.getId());
            }
        }
    }

    float DamageCollisionSystem::notRootedDistance(const PositionComponent &position, const PositionComponent &position2)
    {
        float x = position.x - position2.x;
        float y = position.y - position2.y;
        return x * x + y * y;
    }

    aecs::EntityChanges DamageCollisionSystem::update(aecs::UpdateParams &updateParams)
    {
        (void)updateParams;
        aecs::EntityChanges changes;

        for (size_t i = 0; i < entities.size(); i++) {
            auto *entity = entities[i].get();
            auto &damage = entity->getComponent<DamageCollisionComponent>();
            auto &hp = entity->getComponent<HPComponent>();
            auto &pos = entity->getComponent<PositionComponent>();
            bool hasInvulFrames = entity->hasComponent<InvulComponent>();
            sf::Rect rect = getRect(entity, pos);
            for (size_t j = i + 1; j < entities.size(); j++) {
                auto *entity2 = entities[j].get();
                auto &pos2 = entity2->getComponent<PositionComponent>();
                float distance = notRootedDistance(pos, pos2);
                if (distance > 10000)
                    continue;
                bool hasInvulFrames2 = entity2->hasComponent<InvulComponent>();
                auto &damage2 = entity2->getComponent<DamageCollisionComponent>();
                auto &hp2 = entity2->getComponent<HPComponent>();
                sf::Rect rect2 = getRect(entity2, pos2);
                if (!rect.intersects(rect2) || damage.team == damage2.team)
                    continue;
                if (damage.damage != 0 && damage.invulnerability == 0) {
                    hp2.hp -= damage.damage;
                    if (damage2.type != DamageCollisionComponent::ObjectType::LG_BULLET && hasInvulFrames2)
                        damage2.invulnerability = 5;
                    damage2.damaged = true;
                    changes.editedEntities.insert(entity2->getId());
                }
                if (damage2.damage != 0 && damage.invulnerability == 0) {
                    hp.hp -= damage2.damage;
                    if (damage.type != DamageCollisionComponent::ObjectType::LG_BULLET && hasInvulFrames)
                        damage.invulnerability = 5;
                    damage.damaged = true;
                    changes.editedEntities.insert(entity->getId());
                }
                if (hp2.hp <= 0) {
                    if (_world.getIsServer())
                        changes.deletedEntities.insert(entity2->getId());
                    entities.erase(entities.begin() + j);
                    j--;
                }
                if (hp.hp <= 0) {
                    if (_world.getIsServer())
                        changes.deletedEntities.insert(entity->getId());
                    entities.erase(entities.begin() + i);
                    i--;
                    break;
                }
            }
        }
        addPowerUp(changes);
        return changes;
    }

} // namespace rtype
