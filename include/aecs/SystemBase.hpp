//
// Created by qdesmettre on 05/12/23.
//

#ifndef R_TYPE_SYSTEMBASE_HPP
#define R_TYPE_SYSTEMBASE_HPP

#include "SFML/Graphics.hpp"
#include <map>
#include <memory>
#include <set>
#include <stdexcept>
#include <typeindex>
#include <vector>

namespace aecs
{

    enum MouseInputType {
        MOUSE_MOVE = 0,
        MOUSE_LEFT_CLICK_PRESSED,
        MOUSE_LEFT_CLICK_RELEASED,
        MOUSE_RIGHT_CLICK_PRESSED,
        MOUSE_RIGHT_CLICK_RELEASED,
    };

    class Entity;
    class World;

    typedef std::shared_ptr<Entity> EntityPtr;
    typedef int RenderInput;
    typedef std::vector<std::pair<MouseInputType, sf::Vector2f>> MouseInputs;
    typedef std::vector<RenderInput> ClientInputs;
    typedef std::map<unsigned, ClientInputs> ServerInputs;
    typedef std::pair<ClientInputs, MouseInputs> RenderInputs;

    typedef struct EntityChanges {
        std::set<unsigned int> deletedEntities;
        std::set<unsigned int> editedEntities;
    } EntityChanges;

    typedef struct UpdateParams {
        ServerInputs inputs;
        MouseInputs mouseInputs;
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
        virtual RenderInputs render() = 0;
        [[nodiscard]] virtual bool isOpen() const = 0;
        virtual void close() = 0;
    };

    struct SystemData {
        std::shared_ptr<ISystem> system;
        int priority;
        std::type_index typeIndex;
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
        RenderInputs render() override;
        [[nodiscard]] bool isOpen() const override;
        void close() override;
    };

} // namespace aecs

#endif // R_TYPE_SYSTEMBASE_HPP
