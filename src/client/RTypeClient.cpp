//
// Created by qdesmettre on 05/12/23.
//

#include "rtype/RTypeClient.hpp"
#include "rtype/EntityFactory.hpp"
#include "rtype/systems/client/ClientInputSenderSystem.hpp"
#include "rtype/systems/client/DamageSoundSystem.hpp"
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
#include "rtype/components/MusicComponent.hpp"
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

rtype::RTypeClient::RTypeClient(int renderRefreshRate, int logicRefreshRate, int ac, char **av) :
    _world(false, ac, av),
    _renderRefreshRate(renderRefreshRate),
    _logicRefreshRate(logicRefreshRate),
    _renderSystem(_world.registerRenderSystem<RenderSystem>())
{
    setDecodeMap();
    EntityFactory::setWorld(&_world);
    std::vector<ButtonData> buttons;
    std::map<Input, std::function<void()>> handlers;
    handlers[sf::Keyboard::Enter] = [this]() { _world.goToMenu(1); };
    std::vector<SystemPriority> systems;
    Menu menu(buttons, handlers, systems, [] () {});
    _world.addMenu(menu, 0);
    systems = {_world.makeSystem<ClientServerDataHandlerSystem>(-2),
               _world.makeSystem<ClientInputSenderSystem>(-1),
               _world.makeSystem<ClientPingSystem>(0),
               // commented to show that movement comes from server
               _world.makeSystem<ControlPlayerSystem>(0),
               _world.makeSystem<AnimPlayerSystem>(1),
               _world.makeSystem<AnimSystem>(1),
               _world.makeSystem<PhysicsSystem>(1),
               _world.makeSystem<ParallaxSystem>(1),
               //    _world.registerSystem<BulletSystem>(1);
               _world.makeSystem<DamageCollisionSystem>(1),
               _world.makeSystem<DamageSoundSystem>(1),
               // _world.registerSystem<MonsterGenSystem>(1);
               _world.makeSystem<InvulSystem>(1)};
    std::function<void()> setup = [] () {
        auto &bg = EntityFactory::createBackground(1, sf::Vector2f(8, 0));
        bg.addComponent<MusicComponent>("assets/sounds/music.ogg", 30);
        EntityFactory::createBackground(2, sf::Vector2f(5, 0));
        EntityFactory::createBackground(3, sf::Vector2f(3, 0));
        EntityFactory::createBackground(4, sf::Vector2f(12, 0));
        EntityFactory::createBackground(5, sf::Vector2f(15, 0));
    };
    handlers[sf::Keyboard::Enter] = [this]() { _world.goToMenu(0); };
    Menu game(buttons, handlers, systems, std::move(setup));
    _world.addMenu(game, 1);
    _world.goToMenu(0);
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
