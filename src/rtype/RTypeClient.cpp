//
// Created by qdesmettre on 05/12/23.
//

#include "rtype/RTypeClient.hpp"
#include "rtype/components/PositionComponent.hpp"
#include "rtype/components/SpriteComponent.hpp"
#include "rtype/components/VelocityComponent.hpp"
#include "rtype/components/MyPlayerComponent.hpp"
#include "rtype/components/PlayerComponent.hpp"
#include "rtype/systems/PhysicsSystem.hpp"
#include "rtype/systems/ControlPlayerSystem.hpp"
#include "rtype/systems/AnimPlayerSystem.hpp"
#include <chrono>
#include <thread>

rtype::RTypeClient::RTypeClient(int renderRefreshRate, int logicRefreshRate) :
    _world(),
    _renderRefreshRate(renderRefreshRate),
    _logicRefreshRate(logicRefreshRate),
    _renderSystem(_world.registerRenderSystem<RenderSystem>())
{
    auto &tmp = _world.createEntity();
    tmp.addComponent<SpriteComponent>("assets/sprites/Player2.png", sf::Vector2f{96, 96}, sf::IntRect(0, 0, 96, 96));
    tmp.addComponent<PositionComponent>(0, 0);
    tmp.addComponent<VelocityComponent>(0, 0);
    tmp.addComponent<MyPlayerComponent>();
    tmp.addComponent<PlayerComponent>();

    _world.registerSystem<ControlPlayerSystem>(0);
    _world.registerSystem<AnimPlayerSystem>(0);
    _world.registerSystem<PhysicsSystem>(1);
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
