//
// Created by qdesmettre on 20/12/23.
//

#include "rtype/systems/server/NewConnectionSystem.hpp"
#include "aecs/StaticPacketBuilder.hpp"
#include "rtype/NetworkGlobals.hpp"
#include "rtype/components/ClientAdressComponent.hpp"
#include "rtype/components/ClientPingComponent.hpp"
#include "rtype/components/PlayerComponent.hpp"
#include <iostream>

#ifdef SFML_SYSTEM_WINDOWS
#include <winsock2.h>
#else
#include <sys/socket.h>
#endif

namespace rtype
{

    sf::Socket::Status RTypeListener::listen(unsigned short port, const sf::IpAddress &address)
    {
        close();
        create();

        if ((address == sf::IpAddress::None) || (address == sf::IpAddress::Broadcast))
            return sf::Socket::Error;

        sockaddr_in addr = createAddress(address.toInteger(), port);
#if defined(WIN64) || defined(WIN32) || defined(WINNT)
        setsockopt(getHandle(), SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char *>(&addr), sizeof(addr));
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

    sockaddr_in RTypeListener::createAddress(unsigned int address, unsigned short port)
    {
#if defined(WIN64) || defined(WIN32) || defined(WINNT)
        sockaddr_in addr{.sin_family = AF_INET,
                         .sin_port = htons(port),
                         .sin_addr = {.S_un = {.S_addr = htonl(address)}},
                         .sin_zero = {0}};
#else
        sockaddr_in addr{
            .sin_family = AF_INET, .sin_port = htons(port), .sin_addr = {.s_addr = htonl(address)}, .sin_zero = {0}};
#endif

#if defined(SFML_SYSTEM_MACOS)
        addr.sin_len = sizeof(addr);
#endif

        return addr;
    }

    NewConnectionSystem::NewConnectionSystem(aecs::World &world,
                                             const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
        ALogicSystem(world, entities, {typeid(ClientAdressComponent), typeid(PlayerComponent)}),
        _listener()
    {
        _listener.listen(SERVER_TCP_PORT);
        _listener.setBlocking(false);
    }

    // listen for new connections and send game state to new clients
    aecs::EntityChanges NewConnectionSystem::update(aecs::UpdateParams &updateParams)
    {
        sf::TcpSocket socket;
        aecs::EntityChanges changes;

        socket.setBlocking(false);
        if (_entitiesMap.size() >= 4)
            return {};
        while (_listener.accept(socket) == sf::Socket::Done) {
            auto [isConnected, player] = getClientInfo(socket.getRemoteAddress());
            if (isConnected) {
                std::cout << "Client already connected" << std::endl;
                changes.deletedEntities.push_back(player->getId());
                auto *playerComponent = player->safeGetComponent<PlayerComponent>();
                if (playerComponent)
                    playerComponent->unUsePlayerId();
            }

            std::cout << "New connection from " << socket.getRemoteAddress() << std::endl;
            auto entity = handleClient(socket);
            if (entity)
                updateParams.entityChanges.editedEntities.push_back(entity->getId());
            socket.disconnect();
        }
        return changes;
    }

    std::pair<bool, aecs::Entity *> NewConnectionSystem::getClientInfo(const sf::IpAddress &address)
    {
        for (auto &[_, entity] : _entitiesMap) {
            auto &clientAdress = entity->getComponent<ClientAdressComponent>();
            if (clientAdress.adress == address.toInteger())
                return {true, entity.get()};
        }
        return {false, nullptr};
    }

    aecs::Entity *NewConnectionSystem::handleClient(sf::TcpSocket &socket)
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

        std::cout << "id: " << id << std::endl;

        // send
        if (socket.send(packet) != sf::Socket::Done) {
            std::cerr << "Failed to send game state to new client" << std::endl;
            _world.destroyEntity(playerEntity);
            return nullptr;
        }
        return &playerEntity;
    }
} // namespace rtype
