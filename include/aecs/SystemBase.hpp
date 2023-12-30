//
// Created by qdesmettre on 05/12/23.
//

#ifndef R_TYPE_SYSTEMBASE_HPP
#define R_TYPE_SYSTEMBASE_HPP

#include <map>
#include <memory>
#include <stdexcept>
#include <typeindex>
#include <vector>

#define unused __attribute__((unused))

namespace aecs
{

    class Entity;
    class World;

    typedef std::shared_ptr<Entity> EntityPtr;
    typedef int RenderInput;
    typedef std::vector<RenderInput> ClientInputs;
    typedef std::map<unsigned, ClientInputs> ServerInputs;

    typedef struct EntityChanges {
        std::vector<unsigned int> deletedEntities;
        std::vector<unsigned int> editedEntities;
    } EntityChanges;

    typedef struct UpdateParams {
        ServerInputs inputs;
        float deltaTime = 0;
        EntityChanges entityChanges;
    } UpdateParams;

    class ISystem
    {
      public:
        virtual ~ISystem() = default;

        virtual void onEntityAdded(const aecs::EntityPtr &entity) = 0;
        virtual void onEntityRemoved(const aecs::EntityPtr &entity) = 0;
        virtual void onEntityModified(const aecs::EntityPtr &entity) = 0;

        // For logic systems
        virtual EntityChanges update(aecs::UpdateParams &updateParams) = 0;

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
        void onEntityRemoved(const aecs::EntityPtr &entity) override;
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
                      const std::vector<std::type_index> &componentsNeeded);
        ~ARenderSystem() override = default;

        // For logic systems ONLY
        EntityChanges update(aecs::UpdateParams &) override;
    };

    class ALogicSystem : public ASystem
    {
      public:
        ALogicSystem(aecs::World &world, const std::map<std::size_t, EntityPtr> &entities,
                     const std::vector<std::type_index> &componentsNeeded);
        ~ALogicSystem() override = default;

        // For render systems ONLY
        ClientInputs render() override;
        [[nodiscard]] bool isOpen() const override;
    };

} // namespace aecs

#endif // R_TYPE_SYSTEMBASE_HPP
