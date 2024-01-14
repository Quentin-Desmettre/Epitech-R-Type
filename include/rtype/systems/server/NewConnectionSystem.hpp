//
// Created by qdesmettre on 20/12/23.
//

#ifndef R_TYPE_NEWCONNECTIONSYSTEM_HPP
#define R_TYPE_NEWCONNECTIONSYSTEM_HPP

#include "aecs/SystemBase.hpp"
#include "aecs/World.hpp"
#include <SFML/Network.hpp>

#if defined(WIN64) || defined(WIN32) || defined(WINNT)
    #include <winsock2.h>
    #include <windows.h>
#else
    #include <netinet/in.h>
    #include <sys/socket.h
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
        std::pair<bool, aecs::Entity *> getClientInfo(const sf::IpAddress &address);

        aecs::Entity *handleClient(sf::TcpSocket &socket);

        RTypeListener _listener;
    };
} // namespace rtype

#endif // R_TYPE_NEWCONNECTIONSYSTEM_HPP
