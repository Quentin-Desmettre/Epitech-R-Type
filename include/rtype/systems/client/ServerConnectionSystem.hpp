//
// Created by qdesmettre on 20/12/23.
//

#ifndef R_TYPE_SERVERCONNECTIONSYSTEM_HPP
#define R_TYPE_SERVERCONNECTIONSYSTEM_HPP

#include "SFML/Network.hpp"
#include "aecs/StaticPacketParser.hpp"
#include "aecs/SystemBase.hpp"
#include "aecs/World.hpp"
#include "rtype/NetworkGlobals.hpp"
#include "rtype/components/ClientPingComponent.hpp"
#include "rtype/components/PositionComponent.hpp"
#include <iostream>

namespace rtype
{
    class ServerConnectionSystem : public aecs::ALogicSystem
    {
      public:
        ServerConnectionSystem(aecs::World &world,
                               const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities);

        ~ServerConnectionSystem() override = default;

        /*
         * 1. Connect to server via TCP socket
         * 2. Wait for message, containing game state
         * 3. set game state
         */
        aecs::EntityChanges update(aecs::UpdateParams &updateParams) override;

        [[nodiscard]] bool isConnected() const;

      private:
        bool _connected;
        sf::TcpSocket _socket;
    };
} // namespace rtype

#endif // R_TYPE_SERVERCONNECTIONSYSTEM_HPP
