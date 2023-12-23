//
// Created by qdesmettre on 05/12/23.
//

#include "rtype/RTypeServer.hpp"
#include "rtype/EntityFactory.hpp"
#include "rtype/systems/NewConnectionSystem.hpp"
#include "rtype/systems/PhysicsSystem.hpp"
#include "rtype/systems/ControlPlayerSystem.hpp"
#include "rtype/systems/AnimPlayerSystem.hpp"
#include "rtype/systems/ParallaxSystem.hpp"
#include "rtype/systems/BulletSystem.hpp"
#include "rtype/systems/AnimSystem.hpp"
#include "rtype/systems/DamageCollisionSystem.hpp"
#include "rtype/systems/MonsterGenSystem.hpp"
#include "rtype/systems/MonsterBullet.hpp"
#include "rtype/systems/InvulSystem.hpp"
#include "rtype/systems/ServerUdpSystem.hpp"
#include <chrono>
#include <thread>

rtype::RTypeServer::RTypeServer(int logicRefreshRate) :
    _world(true),
    _logicRefreshRate(logicRefreshRate)
{
    EntityFactory::setWorld(&_world);
    EntityFactory::createBackground(1, sf::Vector2f(8, 0));
    EntityFactory::createBackground(2, sf::Vector2f(5, 0));
    EntityFactory::createBackground(3, sf::Vector2f(3, 0));
    EntityFactory::createBackground(4, sf::Vector2f(12, 0));
    EntityFactory::createBackground(5, sf::Vector2f(15, 0));
    auto &player = EntityFactory::createPlayer(true);
    player.getComponent<rtype::PositionComponent>().x = 100;

    // Network systems
    _world.registerSystem<NewConnectionSystem>(-2);
    _world.registerSystem<ServerUdpSystem>(-1);

    _world.registerSystem<ControlPlayerSystem>(0);
    _world.registerSystem<AnimPlayerSystem>(1);
    _world.registerSystem<AnimSystem>(1);
    _world.registerSystem<PhysicsSystem>(1);
    _world.registerSystem<ParallaxSystem>(1);
    _world.registerSystem<BulletSystem>(1);
    _world.registerSystem<DamageCollisionSystem>(1);
    _world.registerSystem<MonsterGenSystem>(1);
    _world.registerSystem<InvulSystem>(1);
    _world.registerSystem<MonsterBullet>(1);
}

void rtype::RTypeServer::run()
{
    infinteLoop(_logicRefreshRate,
                [this]() {
                    return true;
                },
                [this]() {
                    _world.update();
                });
}

void rtype::RTypeServer::infinteLoop(int refreshRate, std::function<bool()> &&run, std::function<void()> &&function)
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
