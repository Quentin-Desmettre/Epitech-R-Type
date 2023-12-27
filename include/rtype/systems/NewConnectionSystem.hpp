//
// Created by qdesmettre on 20/12/23.
//

#ifndef R_TYPE_NEWCONNECTIONSYSTEM_HPP
#define R_TYPE_NEWCONNECTIONSYSTEM_HPP

#include "aecs/SystemBase.hpp"
#include "aecs/World.hpp"
#include "rtype/NetworkGlobals.hpp"
#include "rtype/components/ClientAdressComponent.hpp"
#include "rtype/components/ClientPingComponent.hpp"
#include "rtype/components/PlayerComponent.hpp"
#include "rtype/components/PositionComponent.hpp"
#include <SFML/Network.hpp>
#include <SFML/System/Err.hpp>
#include <iostream>

#ifdef SFML_SYSTEM_WINDOWS
#include <winsock2.h>
#else
#include <sys/socket.h>
#endif
#include "aecs/StaticPacketBuilder.hpp"
#include <cstring>

#if defined(WIN64) || defined(WIN32) || defined(WINNT)
    #include <winsock2.h>
    #include <windows.h>
#else
    #include <netinet/in.h>
#endif

namespace rtype
{

    class RTypeListener : public sf::TcpListener
    {
      public:
        Socket::Status listen(unsigned short port, const sf::IpAddress &address = sf::IpAddress::Any)
        {
            close();
            create();

            if ((address == sf::IpAddress::None) || (address == sf::IpAddress::Broadcast))
                return sf::Socket::Error;

            sockaddr_in addr = createAddress(address.toInteger(), port);
            #if defined(WIN64) || defined(WIN32) || defined(WINNT)
                setsockopt(getHandle(), SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&addr), sizeof(addr));
            #else
                setsockopt(getHandle(), SOL_SOCKET, SO_REUSEADDR, &addr, sizeof(addr));
            #endif
            if (bind(getHandle(), reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) == -1) {
                sf::err() << "Failed to bind listener socket to port " << port << std::endl;
                return sf::Socket::Error;
            }

            if (::listen(getHandle(), SOMAXCONN) == -1) {
                sf::err() << "Failed to listen to port " << port << std::endl;
                return sf::Socket::Error;
            }

            return sf::Socket::Done;
        }

      protected:
      private:
        sockaddr_in createAddress(unsigned int address, unsigned short port)
        {
            sockaddr_in addr;
            std::memset(&addr, 0, sizeof(addr));
            addr.sin_addr.s_addr = htonl(address);
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);

#if defined(SFML_SYSTEM_MACOS)
            addr.sin_len = sizeof(addr);
#endif

            return addr;
        }
    };

    class NewConnectionSystem : public aecs::ALogicSystem
    {
      public:
        NewConnectionSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
            ALogicSystem(world, entities, {}),
            _listener()
        {
            _listener.listen(SERVER_TCP_PORT);
            _listener.setBlocking(false);
        }

        ~NewConnectionSystem() override = default;

        // listen for new connections and send game state to new clients
        aecs::EntityChanges update(aecs::UpdateParams &updateParams) override
        {
            sf::TcpSocket socket;

            socket.setBlocking(false);
            while (_listener.accept(socket) == sf::Socket::Done) {
                if (isClientAlreadyConnected(socket.getRemoteAddress())) {
                    std::cout << "Client already connected" << std::endl;
                    socket.disconnect();
                    continue;
                }

                std::cout << "New connection from " << socket.getRemoteAddress() << std::endl;
                handleClient(socket);
                socket.disconnect();
            }
            return {};
        }

      private:
        bool isClientAlreadyConnected(const sf::IpAddress &address)
        {
            for (auto &[_, entity] : _entitiesMap) {
                if (!entity->hasComponent<ClientAdressComponent>())
                    continue;
                auto &clientAdress = entity->getComponent<ClientAdressComponent>();
                if (clientAdress.adress == address.toInteger())
                    return true;
            }
            return false;
        }

        void handleClient(sf::TcpSocket &socket)
        {
            // When a new client is connected, create its entity, send game state + its id

            // Get client id
            auto &playerEntity = _world.createEntity();
            // After this line, systems have been notified of the creation of the player, so every component
            // will be added to the entity before this line ends
            playerEntity.addComponent<ClientAdressComponent>(socket.getRemoteAddress().toInteger());
            playerEntity.addComponent<ClientPingComponent>();
            std::size_t id = playerEntity.getComponent<PlayerComponent>().playerId;

            // Build packet
            sf::Packet packet = aecs::StaticPacketBuilder::buildConnectedPacket(id, _world);

            // send
            if (socket.send(packet) != sf::Socket::Done) {
                std::cerr << "Failed to send game state to new client" << std::endl;
                _world.destroyEntity(playerEntity);
            }
        }

        RTypeListener _listener;
    };
} // namespace rtype

#endif // R_TYPE_NEWCONNECTIONSYSTEM_HPP
