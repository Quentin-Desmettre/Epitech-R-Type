//
// Created by Clément Vandeville on 08/01/2024.
//

#include "PongServer.hpp"
#include "ServerInputsSystem.hpp"
#include "NewConnectionSystem.hpp"
#include "DeleteClientSystem.hpp"
#include "PlayerOnConnectionSystem.hpp"
#include "ControlPlayerSystem.hpp"
#include "PhysicsSystem.hpp"

pong::PongServer::PongServer(int logicRefreshRate, int ac, char **av) :
        _world(true, ac, av),
        _logicRefreshRate(logicRefreshRate)
{
    EntityFactory::setWorld(&_world);
//
//    // Network systems
    _world.registerSystem<NewConnectionSystem>(-3);
    _world.registerSystem<ServerInputsSystem>(-2);
    _world.registerSystem<DeleteClientSystem>(-1);
    _world.registerSystem<PlayerOnConnectionSystem>(-1);

    _world.registerSystem<ControlPlayerSystem>(0);
    _world.registerSystem<PhysicsSystem>(1);
//    _world.registerSystem<AnimPlayerSystem>(1);
//    _world.registerSystem<AnimSystem>(1);
//    _world.registerSystem<ParallaxSystem>(1);
//    _world.registerSystem<BulletSystem>(1);
//    _world.registerSystem<DamageCollisionSystem>(1);
//    _world.registerSystem<MonsterGenSystem>(1);
//    _world.registerSystem<InvulSystem>(1);
//    _world.registerSystem<MonsterBullet>(1);
//
//    // Network systems
//    _world.registerSystem<ServerCorrectionsSystem>(1000);
}

void pong::PongServer::run()
{
    infiniteLoop(
            _logicRefreshRate,
            []() {
                return true;
            },
            [this]() {
                _world.update();
            });
}

void pong::PongServer::infiniteLoop(int refreshRate, std::function<bool()> &&run, std::function<void()> &&function)
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
