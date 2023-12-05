//
// Created by qdesmettre on 05/12/23.
//

#ifndef R_TYPE_SYSTEMBASE_HPP
#define R_TYPE_SYSTEMBASE_HPP

#include <memory>
#include <vector>
#include <map>
#include <typeindex>

namespace aecs
{

class Entity;
class World;

typedef std::shared_ptr<Entity> EntityPtr;

class ISystem
{
  public:
    virtual ~ISystem() = default;

    virtual void onEntityAdded(const aecs::EntityPtr &entity) = 0;
    virtual void onEntityRemoved(const aecs::EntityPtr &entity) = 0;
    virtual void onEntityModified(const aecs::EntityPtr &entity) = 0;
};
class ASystem : public ISystem
{
  public:
    ASystem(aecs::World &world, const std::map<std::size_t, EntityPtr> &entities, const std::vector<std::type_index> &componentsNeeded);
    ~ASystem() override = default;

    void onEntityAdded(const aecs::EntityPtr &entity) final;
    void onEntityRemoved(const aecs::EntityPtr &entity) final;
    void onEntityModified(const aecs::EntityPtr &entity) final;

protected:

    aecs::World &_world;
    std::map<std::size_t, aecs::EntityPtr> _entitiesMap;
    std::vector<std::type_index> _componentsNeeded;
};

class ARenderSystem: public ASystem
{
public:
    typedef int RenderInput;
    ARenderSystem(aecs::World &world, const std::map<std::size_t, EntityPtr> &entities, const std::vector<std::type_index> &componentsNeeded):
        ASystem(world, entities, componentsNeeded)
    {
    }
    ~ARenderSystem() override = default;

    virtual std::vector<RenderInput> render() = 0;
    [[nodiscard]] virtual bool isOpen() const = 0;
};

class ALogicSystem: public ASystem
{
  public:
    ALogicSystem(aecs::World &world, const std::map<std::size_t, EntityPtr> &entities, const std::vector<std::type_index> &componentsNeeded):
        ASystem(world, entities, componentsNeeded)
    {
    }
    ~ALogicSystem() override = default;

    virtual void update(const std::vector<ARenderSystem::RenderInput> &inputs) = 0;
};

} // namespace aecs

#endif // R_TYPE_SYSTEMBASE_HPP
