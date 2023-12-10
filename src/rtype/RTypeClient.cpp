//
// Created by qdesmettre on 05/12/23.
//

#include "rtype/RTypeClient.hpp"
#include "rtype/components/PositionComponent.hpp"
#include "rtype/components/SpriteComponent.hpp"
#include "rtype/components/VelocityComponent.hpp"
#include "rtype/components/MyPlayerComponent.hpp"
#include "rtype/components/ParallaxComponent.hpp"
#include "rtype/components/PlayerComponent.hpp"
#include "rtype/systems/PhysicsSystem.hpp"
#include "rtype/systems/ControlPlayerSystem.hpp"
#include "rtype/systems/AnimPlayerSystem.hpp"
#include "rtype/systems/ParallaxSystem.hpp"
#include "rtype/systems/BulletSystem.hpp"
#include <chrono>
#include <thread>

rtype::RTypeClient::RTypeClient(int renderRefreshRate, int logicRefreshRate) :
    _world(),
    _renderRefreshRate(renderRefreshRate),
    _logicRefreshRate(logicRefreshRate),
    _renderSystem(_world.registerRenderSystem<RenderSystem>())
{
    auto &back1 = _world.createEntity();
    back1.addComponent<SpriteComponent>("assets/sprites/Back1.png", sf::Vector2f{1088, 640}, sf::IntRect {0, 0, 272 * 2 , 160});
    back1.addComponent<PositionComponent>(0, 0);
    back1.addComponent<ParallaxComponent>(sf::Vector2f(8, 0));
    auto &back2 = _world.createEntity();
    back2.addComponent<SpriteComponent>("assets/sprites/Back2.png", sf::Vector2f{1088, 640}, sf::IntRect {0, 0,272 * 2 , 160});
    back2.addComponent<PositionComponent>(0, 0);
    back2.addComponent<ParallaxComponent>(sf::Vector2f(5, 0));
    auto &back3 = _world.createEntity();
    back3.addComponent<SpriteComponent>("assets/sprites/Back3.png", sf::Vector2f{1088, 640}, sf::IntRect {0, 0,272 * 2 , 160});
    back3.addComponent<PositionComponent>(0, 0);
    back3.addComponent<ParallaxComponent>(sf::Vector2f(3, 0));
    auto &back4 = _world.createEntity();
    back4.addComponent<SpriteComponent>("assets/sprites/Back4.png", sf::Vector2f{1088, 640}, sf::IntRect {0, 0,272 * 2 , 160});
    back4.addComponent<PositionComponent>(0, 0);
    back4.addComponent<ParallaxComponent>(sf::Vector2f(12, 0));
    auto &back5 = _world.createEntity();
    back5.addComponent<SpriteComponent>("assets/sprites/Back5.png", sf::Vector2f{1088, 640}, sf::IntRect {0, 0,272 * 2 , 160});
    back5.addComponent<PositionComponent>(0, 0);
    back5.addComponent<ParallaxComponent>(sf::Vector2f(15, 0));
    auto &player = _world.createEntity();
    player.addComponent<SpriteComponent>("assets/sprites/PlayerNew.png", sf::Vector2f{96, 96}, sf::IntRect(0, 0, 32, 32));
    player.addComponent<PositionComponent>(0, 0);
    player.addComponent<VelocityComponent>(0, 0);
    player.addComponent<MyPlayerComponent>();
    player.addComponent<PlayerComponent>();

    _world.registerSystem<ControlPlayerSystem>(0);
    _world.registerSystem<AnimPlayerSystem>(1);
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
