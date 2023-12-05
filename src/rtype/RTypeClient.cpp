//
// Created by qdesmettre on 05/12/23.
//

#include "RTypeClient.hpp"
#include "rtype/components/SpriteComponent.hpp"
#include <chrono>
#include <thread>

rtype::RTypeClient::RTypeClient(int renderRefreshRate, int logicRefreshRate) :
    _world(),
    _renderRefreshRate(renderRefreshRate),
    _logicRefreshRate(logicRefreshRate),
    _renderSystem(_world.registerRenderSystem<RenderSystem>())
{
    auto &tmp = _world.createEntity();
    tmp.addComponent<SpriteComponent>("assets/sprites/ship.png", sf::Vector2f{200, 200});
}

void rtype::RTypeClient::run()
{
    // Launch a thread for logic
    std::thread{
        &RTypeClient::infinteLoop, _logicRefreshRate,
        [this]() {
            return _renderSystem.isOpen();
        },
        [this]() {
            _world.update();
        }}.detach();

    // Render in the main thread
    infinteLoop(
        _renderRefreshRate,
        [this]() {
            return _renderSystem.isOpen();
        },
        [this]() {
            _renderSystem.render();
        });
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
