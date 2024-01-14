//
// Created by qdesmettre on 20/12/23.
//

#ifndef R_TYPE_CLIENTSERVERDATAHANDLERSYSTEM_HPP
#define R_TYPE_CLIENTSERVERDATAHANDLERSYSTEM_HPP

#include "SFML/Network.hpp"
#include "ServerConnectionSystem.hpp"
#include "StaticPacketBuilder.hpp"
#include "StaticPacketParser.hpp"
#include "SystemBase.hpp"
#include "World.hpp"
#include "NetworkGlobals.hpp"
#include "ClientPingComponent.hpp"
#include "PositionComponent.hpp"
#include <cassert>
#include <chrono>
#include <queue>

// TODO: put it in constructor
// Delay before doing a packet, in ms
//static const int BUFFER_DELAY = 30;

/**
 * @brief System that handle the data received from the server
 */
class ClientServerDataHandlerSystem : public aecs::ALogicSystem
{
    public:
        ClientServerDataHandlerSystem(aecs::World &world,
                                      const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities);

        ~ClientServerDataHandlerSystem() override = default;

        aecs::EntityChanges update(aecs::UpdateParams &updateParams) override;

    private:
        sf::UdpSocket _socket;
        ServerConnectionSystem _tcpHandshakeSystem;
        std::queue<std::pair<sf::Packet, std::chrono::milliseconds>> _packets;
        unsigned _maxReceivedTick;
};

#endif // R_TYPE_CLIENTSERVERDATAHANDLERSYSTEM_HPP
