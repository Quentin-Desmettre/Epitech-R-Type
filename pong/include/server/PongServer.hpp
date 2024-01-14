//
// Created by Clément Vandeville on 08/01/2024.
//

#ifndef PONG_PONGSERVER_HPP
#define PONG_PONGSERVER_HPP

#include "EntityFactory.hpp"
#include "common/ecs/World.hpp"
#include <functional>
#include <thread>

namespace pong
{

    class PongServer
    {
        public:
            PongServer(int logicRefreshRate, int ac, char **av);
            ~PongServer() = default;

            void run();

        private:
            static void infiniteLoop(int refreshRate, std::function<bool()> &&run, std::function<void()> &&function);
            aecs::World _world;
            int _logicRefreshRate;
    };

} // namespace rtype

#endif //PONG_PONGSERVER_HPP
