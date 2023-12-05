//
// Created by qdesmettre on 05/12/23.
//

#ifndef R_TYPE_RTYPECLIENT_HPP
#define R_TYPE_RTYPECLIENT_HPP

#include "aecs/World.hpp"
#include "rtype/systems/RenderSystem.hpp"
#include <functional>

namespace rtype
{

    class RTypeClient
    {
      public:
        RTypeClient(int renderRefreshRate, int logicRefreshRate);
        ~RTypeClient() = default;

        void run();

      private:
        static void infinteLoop(int refreshRate, std::function<bool()> &&run, std::function<void()> &&function);
        aecs::World _world;
        int _renderRefreshRate;
        int _logicRefreshRate;
        RenderSystem &_renderSystem;
    };

} // namespace rtype

#endif // R_TYPE_RTYPECLIENT_HPP
