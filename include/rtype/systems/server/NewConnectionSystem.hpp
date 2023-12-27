//
// Created by qdesmettre on 20/12/23.
//

#ifndef R_TYPE_NEWCONNECTIONSYSTEM_HPP
#define R_TYPE_NEWCONNECTIONSYSTEM_HPP

#include "SFML/Network.hpp"
#include "SFML/System/Err.hpp"
#include "aecs/SystemBase.hpp"
#include "aecs/World.hpp"
#include "rtype/NetworkGlobals.hpp"
#include "rtype/components/ClientAdressComponent.hpp"
#include "rtype/components/ClientPingComponent.hpp"
#include "rtype/components/PlayerComponent.hpp"
#include "rtype/components/PositionComponent.hpp"
#include <iostream>

#ifdef SFML_SYSTEM_WINDOWS
#include <winsock2.h>
#else
#include <sys/socket.h>
#endif
#include "aecs/StaticPacketBuilder.hpp"
#include <cstring>

#if defined(WIN64) || defined(WIN32) || defined(WINNT)
#include <windows.h>
#include <winsock2.h>
#else
#include <netinet/in.h>
#endif

namespace rtype
{

    class RTypeListener : public sf::TcpListener
    {
      public:
        Socket::Status listen(unsigned short port, const sf::IpAddress &address = sf::IpAddress::Any);

      protected:
      private:
        static sockaddr_in createAddress(unsigned int address, unsigned short port);
    };

    class NewConnectionSystem : public aecs::ALogicSystem
    {
      public:
        NewConnectionSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities);

        ~NewConnectionSystem() override = default;

        // listen for new connections and send game state to new clients
        aecs::EntityChanges update(aecs::UpdateParams &updateParams) override;

      private:
        bool isClientAlreadyConnected(const sf::IpAddress &address);

        aecs::Entity *handleClient(sf::TcpSocket &socket);

        RTypeListener _listener;
    };
} // namespace rtype

#endif // R_TYPE_NEWCONNECTIONSYSTEM_HPP
