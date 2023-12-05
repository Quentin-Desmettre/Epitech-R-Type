//
// Created by qdesmettre on 05/12/23.
//

#ifndef R_TYPE_SYSTEMBASE_HPP
#define R_TYPE_SYSTEMBASE_HPP

#include <memory>
#include <vector>

namespace aecs
{

class Entity;

typedef std::shared_ptr<Entity> EntityPtr;

class ISystem
{
  public:
    virtual ~ISystem() = default;

    virtual void onEntityAdded(const aecs::EntityPtr &entity) = 0;
    virtual void onEntityRemoved(const aecs::EntityPtr &entity) = 0;
    virtual void onEntityModified(const aecs::EntityPtr &entity) = 0;
};

class IRenderSystem : public ISystem
{
  public:
    typedef int RenderInput;
    ~IRenderSystem() override = default;
    virtual std::vector<RenderInput> render() = 0;
    [[nodiscard]] virtual bool isOpen() const = 0;
};

class ILogicSystem : public ISystem
{
  public:
    ~ILogicSystem() override = default;
    virtual void update(const std::vector<IRenderSystem::RenderInput> &inputs) = 0;
};

} // namespace aecs

#endif // R_TYPE_SYSTEMBASE_HPP
