//
// Created by qdesmettre on 05/12/23.
//

#ifndef R_TYPE_RTYPESERVER_HPP
#define R_TYPE_RTYPESERVER_HPP

#include "aecs/World.hpp"
#include "rtype/EntityFactory.hpp"
#include <functional>

namespace rtype
{

    class RTypeServer
    {
      public:
        RTypeServer(int logicRefreshRate);
        ~RTypeServer() = default;

        void run();

      private:
        static void infinteLoop(int refreshRate, std::function<bool()> &&run, std::function<void()> &&function);
        aecs::World _world;
        int _logicRefreshRate;
    };

} // namespace rtype

#endif // R_TYPE_RTYPESERVER_HPP
