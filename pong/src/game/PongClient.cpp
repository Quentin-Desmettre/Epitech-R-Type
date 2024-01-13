//
// Created by Clément Vandeville on 09/01/2024.
//

#include "PongClient.hpp"
#include "ClientServerDataHandlerSystem.hpp"
#include "ClientInputSenderSystem.hpp"
#include "ClientPingSystem.hpp"
#include "ControlPlayerSystem.hpp"
#include "PhysicsSystem.hpp"

void PongClient::setDecodeMap()
{
    _world.addDecodeMap(typeid(PlayerComponent), [](aecs::Entity &entity, const std::vector<std::byte> &data) {
        entity.addComponent<PlayerComponent>();
        auto &component = entity.getComponent<PlayerComponent>();
        component.decode(data);
        EntityFactory::toPlayer(entity);
    });
}

PongClient::PongClient(int renderRefreshRate, int logicRefreshRate, int ac, char **av) :
        _world(false, ac, av),
        _renderRefreshRate(renderRefreshRate),
        _logicRefreshRate(logicRefreshRate),
        _renderSystem(_world.registerRenderSystem<RenderSystem>())
{
    setDecodeMap();
    EntityFactory::setWorld(&_world);
    auto &bg = EntityFactory::createBackground();
//    TODO  REMETTRE LA MUSIQUE
//    bg.addComponent<MusicComponent>("assets/sounds/music.ogg", 30);

//    EntityFactory::createScore();
//    EntityFactory::createHealth();

    // Network systems
    _world.registerSystem<ClientServerDataHandlerSystem>(-2);
    _world.registerSystem<ClientInputSenderSystem>(-1);
    _world.registerSystem<ClientPingSystem>(0);

    // commented to show that movement comes from server
    _world.registerSystem<ControlPlayerSystem>(0);
    _world.registerSystem<PhysicsSystem>(1);
//    _world.registerSystem<AnimPlayerSystem>(1);
//    _world.registerSystem<AnimSystem>(1);
//    _world.registerSystem<ParallaxSystem>(1);
////    _world.registerSystem<BulletSystem>(1);
//    _world.registerSystem<DamageCollisionSystem>(1);
//    _world.registerSystem<DamageSoundSystem>(1);
//    // _world.registerSystem<MonsterGenSystem>(1);
//    _world.registerSystem<InvulSystem>(1);
//    _world.registerSystem<ClientScoreSystem>(1);
//    _world.registerSystem<ClientHealthSystem>(1);
}

void PongClient::run()
{
    // Launch a thread for logic
    std::thread _logicThread = std::thread{&PongClient::infiniteLoop, _logicRefreshRate,
                                           [this]() {
                                               return _renderSystem.isOpen();
                                           },
                                           [this]() {
                                               _world.update();
                                           }};

    // Render in the main thread
    infiniteLoop(
            _renderRefreshRate,
            [this]() {
                return _renderSystem.isOpen();
            },
            [this]() {
                _world.render();
            });
    _logicThread.join();
}

void PongClient::infiniteLoop(int refreshRate, std::function<bool()> &&run, std::function<void()> &&function)
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

