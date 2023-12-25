//
// Created by qdesmettre on 20/12/23.
//

#ifndef R_TYPE_NEWCONNECTIONSYSTEM_HPP
#define R_TYPE_NEWCONNECTIONSYSTEM_HPP

#include "aecs/SystemBase.hpp"
#include "aecs/World.hpp"
#include "rtype/NetworkGlobals.hpp"
#include "rtype/components/ClientAdressComponent.hpp"
#include "rtype/components/ClientPortComponent.hpp"
#include "rtype/components/MyPlayerComponent.hpp"
#include "rtype/components/PositionComponent.hpp"
#include <SFML/Network.hpp>
#include <SFML/System/Err.hpp>
#include <iostream>
#ifdef SFML_SYSTEM_WINDOWS
#include <winsock2.h>
#else
#include <sys/socket.h>
#endif
#include <cstring>
#include <netinet/in.h>

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
            setsockopt(getHandle(), SOL_SOCKET, SO_REUSEADDR, &addr, sizeof(addr));
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
        aecs::EntityChanges update(const aecs::UpdateParams &updateParams) override
        {
            sf::TcpSocket socket;

            socket.setBlocking(false);
            while (_listener.accept(socket) == sf::Socket::Done) {
                std::cout << "New connection from " << socket.getRemoteAddress() << std::endl;
                sf::Packet packet;
                for (auto &[_, entity] : _entitiesMap) {
                    if (entity->hasComponent<MyPlayerComponent>()) {
                        auto &posComponent = entity->getComponent<PositionComponent>();
                        packet << posComponent.x << posComponent.y;
                        break;
                    }
                }
                socket.send(packet);
                aecs::Entity &entity = _world.createEntity();
                entity.addComponent<ClientAdressComponent>(socket.getRemoteAddress().toInteger());
                // TODO: get client port and add it to entity via a ClientPortComponent instead of hardcoding 53002
                // (CLIENT_UDP_PORT)
                entity.addComponent<ClientPortComponent>(CLIENT_INPUTS_PORT);
                socket.disconnect();
            }
            return {};
        }

      private:
        RTypeListener _listener;
    };
} // namespace rtype

#endif // R_TYPE_NEWCONNECTIONSYSTEM_HPP
