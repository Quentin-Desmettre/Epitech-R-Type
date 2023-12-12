//
// Created by qdesmettre on 05/12/23.
//

#include "rtype/RTypeClient.hpp"
#include "rtype/systems/PhysicsSystem.hpp"
#include "rtype/systems/ControlPlayerSystem.hpp"
#include "rtype/systems/AnimPlayerSystem.hpp"
#include "rtype/systems/ParallaxSystem.hpp"
#include "rtype/systems/BulletSystem.hpp"
#include "rtype/systems/AnimSystem.hpp"
#include <chrono>
#include <thread>
#include "rtype/EntityFactory.hpp"

rtype::RTypeClient::RTypeClient(int renderRefreshRate, int logicRefreshRate) :
    _world(),
    _renderRefreshRate(renderRefreshRate),
    _logicRefreshRate(logicRefreshRate),
    _renderSystem(_world.registerRenderSystem<RenderSystem>())
{
    EntityFactory::setWorld(&_world);
    EntityFactory::createBackground(1, sf::Vector2f(8, 0));
    EntityFactory::createBackground(2, sf::Vector2f(5, 0));
    EntityFactory::createBackground(3, sf::Vector2f(3, 0));
    EntityFactory::createBackground(4, sf::Vector2f(12, 0));
    EntityFactory::createBackground(5, sf::Vector2f(15, 0));
    auto &player = _world.createEntity();
    EntityFactory::createPlayer(true);
    for (int i = 0; i < 6; i++)
        EntityFactory::createEnemy(sf::Vector2f(100, 100 * i), sf::Vector2f(-8, 0));
    _world.registerSystem<ControlPlayerSystem>(0);
    _world.registerSystem<AnimPlayerSystem>(1);
    _world.registerSystem<AnimSystem>(1);
    _world.registerSystem<PhysicsSystem>(1);
    _world.registerSystem<ParallaxSystem>(1);
    _world.registerSystem<BulletSystem>(1);
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
