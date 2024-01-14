//
// Created by Clément Vandeville on 09/01/2024.
//

#ifndef PONG_PONGCLIENT_HPP
#define PONG_PONGCLIENT_HPP

#include "World.hpp"
#include "RenderSystem.hpp"
#include "PlayerComponent.hpp"
#include <thread>

class PongClient {
    public:
        PongClient(int renderRefreshRate, int logicRefreshRate, int ac, char **av);
        ~PongClient() = default;

        void run();

    private:
        void setDecodeMap();
        static void infiniteLoop(int refreshRate, std::function<bool()> &&run, std::function<void()> &&function);
        aecs::World _world;
        int _renderRefreshRate;
        int _logicRefreshRate;
        RenderSystem &_renderSystem;
};


#endif //PONG_PONGCLIENT_HPP
