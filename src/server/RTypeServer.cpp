//
// Created by qdesmettre on 05/12/23.
//

#include "rtype/RTypeServer.hpp"
#include "rtype/EntityFactory.hpp"
#include "rtype/systems/server/DeleteClientSystem.hpp"
#include "rtype/systems/server/MapSystem.hpp"
#include "rtype/systems/server/NewConnectionSystem.hpp"
#include "rtype/systems/server/PlayerOnConnectionSystem.hpp"
#include "rtype/systems/server/PowerSystem.hpp"
#include "rtype/systems/server/BulletGenSystem.hpp"
#include "rtype/systems/server/ServerCorrectionsSystem.hpp"
#include "rtype/systems/server/ServerInputsSystem.hpp"
#include "rtype/systems/shared/AnimPlayerSystem.hpp"
#include "rtype/systems/shared/AnimSystem.hpp"
#include "rtype/systems/shared/BossSystem.hpp"
#include "rtype/systems/shared/BulletSystem.hpp"
#include "rtype/systems/shared/ControlPlayerSystem.hpp"
#include "rtype/systems/shared/DamageCollisionSystem.hpp"
#include "rtype/systems/shared/InvulSystem.hpp"
#include "rtype/systems/shared/MonsterDie.hpp"
#include "rtype/systems/shared/MonsterGenSystem.hpp"
#include "rtype/systems/shared/NodeMonsterSystem.hpp"
#include "rtype/systems/shared/ParallaxSystem.hpp"
#include "rtype/systems/shared/PhysicsSystem.hpp"
#include <chrono>
#include <thread>

rtype::RTypeServer::RTypeServer(int logicRefreshRate, int ac, char **av) :
    _world(true, ac, av),
    _logicRefreshRate(logicRefreshRate)
{
    EntityFactory::setWorld(&_world);

    // Network systems
    _world.registerSystem<NewConnectionSystem>(-3);
    _world.registerSystem<ServerInputsSystem>(-2);
    _world.registerSystem<DeleteClientSystem>(-1);
    _world.registerSystem<PlayerOnConnectionSystem>(-1);

    _world.registerSystem<ControlPlayerSystem>(0);
    _world.registerSystem<AnimPlayerSystem>(1);
    _world.registerSystem<AnimSystem>(1);
    _world.registerSystem<PhysicsSystem>(2);
    _world.registerSystem<ParallaxSystem>(1);
    _world.registerSystem<BulletSystem>(1);
    _world.registerSystem<DamageCollisionSystem>(1);
    _world.registerSystem<MapSystem>(1);
    _world.registerSystem<MonsterGenSystem>(1);
    _world.registerSystem<InvulSystem>(1);
    _world.registerSystem<MonsterDie>(1);
    _world.registerSystem<PowerSystem>(2);
    _world.registerSystem<NodeMonsterSystem>(1);
    _world.registerSystem<BulletGenSystem>(1);
    _world.registerSystem<BossSystem>(1);

    // Network systems
    _world.registerSystem<ServerCorrectionsSystem>(1000);
}

void rtype::RTypeServer::run()
{
    infinteLoop(
        _logicRefreshRate,
        []() {
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
