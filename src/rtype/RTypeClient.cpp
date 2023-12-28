//
// Created by qdesmettre on 05/12/23.
//

#include "rtype/RTypeClient.hpp"
#include "rtype/EntityFactory.hpp"
#include "rtype/systems/client/ClientInputSenderSystem.hpp"
#include "rtype/systems/client/ClientPingSystem.hpp"
#include "rtype/systems/client/ClientServerDataHandlerSystem.hpp"
#include "rtype/systems/shared/AnimPlayerSystem.hpp"
#include "rtype/systems/shared/AnimSystem.hpp"
#include "rtype/systems/shared/BulletSystem.hpp"
#include "rtype/systems/shared/ControlPlayerSystem.hpp"
#include "rtype/systems/shared/DamageCollisionSystem.hpp"
#include "rtype/systems/shared/InvulSystem.hpp"
#include "rtype/systems/shared/MonsterBullet.hpp"
#include "rtype/systems/shared/MonsterGenSystem.hpp"
#include "rtype/systems/shared/ParallaxSystem.hpp"
#include "rtype/systems/shared/PhysicsSystem.hpp"
#include <chrono>
#include <thread>

#include "rtype/components/BulletComponent.hpp"
#include "rtype/components/MonsterComponent.hpp"
#include "rtype/components/PlayerComponent.hpp"
void rtype::RTypeClient::setDecodeMap()
{
    _world.addDecodeMap(typeid(PlayerComponent), [](aecs::Entity &entity, const std::vector<std::byte> &data) {
        entity.addComponent<PlayerComponent>();
        auto &component = entity.getComponent<PlayerComponent>();
        component.decode(data);
        EntityFactory::toPlayer(entity);
    });
    _world.addDecodeMap(typeid(BulletComponent), [](aecs::Entity &entity, const std::vector<std::byte> &data) {
        entity.addComponent<BulletComponent>();
        auto &component = entity.getComponent<BulletComponent>();
        component.decode(data);
        EntityFactory::toBullet(entity);
    });
    _world.addDecodeMap(typeid(MonsterComponent), [](aecs::Entity &entity, const std::vector<std::byte> &data) {
        entity.addComponent<MonsterComponent>();
        auto &component = entity.getComponent<MonsterComponent>();
        component.decode(data);
        EntityFactory::toEnemy(entity);
    });
}

rtype::RTypeClient::RTypeClient(int renderRefreshRate, int logicRefreshRate) :
    _world(),
    _renderRefreshRate(renderRefreshRate),
    _logicRefreshRate(logicRefreshRate),
    _renderSystem(_world.registerRenderSystem<RenderSystem>())
{
    setDecodeMap();
    EntityFactory::setWorld(&_world);
    EntityFactory::createBackground(1, sf::Vector2f(8, 0));
    EntityFactory::createBackground(2, sf::Vector2f(5, 0));
    EntityFactory::createBackground(3, sf::Vector2f(3, 0));
    EntityFactory::createBackground(4, sf::Vector2f(12, 0));
    EntityFactory::createBackground(5, sf::Vector2f(15, 0));

    // Network systems
    _world.registerSystem<ClientServerDataHandlerSystem>(-2);
    _world.registerSystem<ClientInputSenderSystem>(-1);
    _world.registerSystem<ClientPingSystem>(0);

    // commented to show that movement comes from server
    _world.registerSystem<ControlPlayerSystem>(0);
    _world.registerSystem<AnimPlayerSystem>(1);
    _world.registerSystem<AnimSystem>(1);
    _world.registerSystem<PhysicsSystem>(1);
    _world.registerSystem<ParallaxSystem>(1);
//    _world.registerSystem<BulletSystem>(1);
    _world.registerSystem<DamageCollisionSystem>(1);
    // _world.registerSystem<MonsterGenSystem>(1);
    _world.registerSystem<InvulSystem>(1);
}

void rtype::RTypeClient::run()
{
    // Launch a thread for logic
    std::thread _logicThread = std::thread{&RTypeClient::infinteLoop, _logicRefreshRate,
                                           [this]() {
                                               return _renderSystem.isOpen();
                                           },
                                           [this]() {
                                               _world.update();
                                           }};

    // Render in the main thread
    infinteLoop(
        _renderRefreshRate,
        [this]() {
            return _renderSystem.isOpen();
        },
        [this]() {
            _world.render();
        });
    _logicThread.join();
}

void rtype::RTypeClient::infinteLoop(int refreshRate, std::function<bool()> &&run, std::function<void()> &&function)
{
    while (run()) {
        // Make sure that each loop takes 1/refreshRate seconds
        auto start = std::chrono::high_resolution_clock::now();

        // Call the function
        function();

        // Make sure that each loop takes 1/refreshRate seconds
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        std::this_thread::sleep_for(std::chrono::nanoseconds(1000000000 / refreshRate) - duration);
    }
}
