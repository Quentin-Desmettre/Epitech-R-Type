//
// Created by qdesmettre on 05/12/23.
//

#ifndef R_TYPE_SYSTEMBASE_HPP
#define R_TYPE_SYSTEMBASE_HPP

#include <map>
#include <memory>
#include <typeindex>
#include <vector>

namespace aecs
{

    class Entity;
    class World;

    typedef std::shared_ptr<Entity> EntityPtr;
    typedef int RenderInput;
    typedef std::vector<RenderInput> ClientInputs;
    typedef std::map<unsigned, ClientInputs> ServerInputs;

#define MY_INPUTS(serverInputs)                                                                                        \
    ((serverInputs).find(0) != (serverInputs).end() ? (serverInputs).at(0) : aecs::ClientInputs())

    typedef struct UpdateParams {
        ServerInputs inputs;
        float deltaTime;
    } UpdateParams;


    typedef struct EntityChanges {
        EntityChanges() = default;
        std::vector<unsigned int> deletedEntities;
        std::vector<unsigned int> editedEntities;
    } EntityChanges;

    class ISystem
    {
      public:
        virtual ~ISystem() = default;

        virtual void onEntityAdded(const aecs::EntityPtr &entity) = 0;
        virtual void onEntityRemoved(const aecs::EntityPtr &entity) = 0;
        virtual void onEntityModified(const aecs::EntityPtr &entity) = 0;

        // For logic systems
        virtual EntityChanges update(const UpdateParams &updateParams) = 0;

        // For render systems
        virtual ClientInputs render() = 0;
        [[nodiscard]] virtual bool isOpen() const = 0;
    };

    class ASystem : public ISystem
    {
      public:
        ASystem(aecs::World &world, const std::map<std::size_t, EntityPtr> &entities,
                const std::vector<std::type_index> &componentsNeeded);
        ~ASystem() override = default;

        void onEntityAdded(const aecs::EntityPtr &entity) override;
        void onEntityRemoved(const aecs::EntityPtr &entity) final;
        void onEntityModified(const aecs::EntityPtr &entity) override;

      protected:
        aecs::World &_world;
        std::map<std::size_t, aecs::EntityPtr> _entitiesMap;
        std::vector<std::type_index> _componentsNeeded;
    };

    class ARenderSystem : public ASystem
    {
      public:
        ARenderSystem(aecs::World &world, const std::map<std::size_t, EntityPtr> &entities,
                      const std::vector<std::type_index> &componentsNeeded) :
            ASystem(world, entities, componentsNeeded)
        {
        }
        ~ARenderSystem() override = default;

        // For logic systems ONLY
        EntityChanges update(const UpdateParams &updateParams) override
        {
            throw std::runtime_error("IRenderSystem::update() should not be called");
        }
    };

    class ALogicSystem : public ASystem
    {
      public:
        ALogicSystem(aecs::World &world, const std::map<std::size_t, EntityPtr> &entities,
                     const std::vector<std::type_index> &componentsNeeded) :
            ASystem(world, entities, componentsNeeded)
        {
        }
        ~ALogicSystem() override = default;

        // For render systems ONLY
        ClientInputs render() override
        {
            throw std::runtime_error("ILogicSystem::render() should not be called");
        }
        [[nodiscard]] bool isOpen() const override
        {
            throw std::runtime_error("ILogicSystem::isOpen() should not be called");
        }
    };

} // namespace aecs

#endif // R_TYPE_SYSTEMBASE_HPP
