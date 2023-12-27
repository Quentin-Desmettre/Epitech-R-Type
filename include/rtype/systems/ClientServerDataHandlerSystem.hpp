//
// Created by qdesmettre on 20/12/23.
//

#ifndef R_TYPE_CLIENTSERVERDATAHANDLERSYSTEM_HPP
#define R_TYPE_CLIENTSERVERDATAHANDLERSYSTEM_HPP

#include "aecs/StaticPacketBuilder.hpp"
#include "aecs/StaticPacketParser.hpp"
#include "aecs/SystemBase.hpp"
#include "aecs/World.hpp"
#include "rtype/NetworkGlobals.hpp"
#include "rtype/components/ClientPingComponent.hpp"
#include "rtype/components/MyPlayerComponent.hpp"
#include "rtype/components/PositionComponent.hpp"
#include "rtype/systems/ServerConnectionSystem.hpp"
#include <SFML/Network.hpp>
#include <cassert>
#include <chrono>
#include <queue>

namespace rtype
{

    // TODO: put it in constructor
    // Delay before doing a packet, in ms
    static const int BUFFER_DELAY = 30;

    /**
     * @brief System that handle the data received from the server
     */
    class ClientServerDataHandlerSystem : public aecs::ALogicSystem
    {
      public:
        ClientServerDataHandlerSystem(aecs::World &world,
                                      const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
            ALogicSystem(world, entities, {typeid(ClientPingComponent)}),
            _maxReceivedTick(0),
            _tcpHandshakeSystem(world, entities)
        {
            _socket.bind(CLIENT_CORRECTIONS_PORT);
            _socket.setBlocking(false);
        }

        ~ClientServerDataHandlerSystem() override = default;

        aecs::EntityChanges update(aecs::UpdateParams &updateParams) override
        {
            _tcpHandshakeSystem.update(updateParams);
            if (!_tcpHandshakeSystem.isConnected())
                return {};

            // Receive a packet
            sf::Packet packet;
            sf::IpAddress sender;
            unsigned short port;

            sf::Socket::Status status = _socket.receive(packet, sender, port);

            // If no packet, return
            if (status != sf::Socket::Done)
                return {};

            // If packet:
            // - send pong
            // - check if tick has already been checked
            // - push to queue
            // - do packet received X ms in the past

            aecs::StaticPacketParser::ParsedData parsed = aecs::StaticPacketParser::parsePacket(packet, 0);

            if (parsed.type == aecs::SERVER_PONG)
                return {};
            if (parsed.type != aecs::GAME_CHANGES) {
                std::cerr << "Unexpected packet type" << std::endl;
                return {};
            }

            // for (auto &[_, entity] : _entitiesMap) {
            //     if (entity->hasComponent<rtype::MyPlayerComponent>()) {
            //         auto &position = entity->getComponent<rtype::PositionComponent>();
            //         packet >> position.x >> position.y;
            //         std::cout << "x: " << position.x << " y: " << position.y << std::endl;
            //         break;
            //     }
            // }

            // Send pong with tick
            unsigned tick = parsed.tick;
            sf::Packet pongPacket = aecs::StaticPacketBuilder::buildClientPongPacket(std::max(tick, _maxReceivedTick));
            _socket.send(pongPacket, sender, SERVER_INPUTS_PORT);

            // Reset ping clock
            for (auto &[_, entity] : _entitiesMap)
                entity->getComponent<ClientPingComponent>().clock.restart();

            // Do the changes
            // _world.load(parsed.entityChanges.back());

            //            // Check if tick has already been checked
            //            if (tick <= _maxReceivedTick)
            //                return {};
            //
            //            // Push to queue
            //            auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
            //                std::chrono::system_clock::now().time_since_epoch());
            //            _maxReceivedTick = tick;
            //            _packets.emplace(packet, now);
            //
            //            // Do packet received X ms in the past
            //            if (_packets.empty())
            //                return {};
            //            auto &[firstPacket, timeReceived] = _packets.front();
            //            if (now - timeReceived < std::chrono::milliseconds(BUFFER_DELAY))
            //                return {};
            //            _packets.pop();
            //            PacketHandler::handle(_world, firstPacket);
            return {};
        }

      private:
        sf::UdpSocket _socket;
        ServerConnectionSystem _tcpHandshakeSystem;
        std::queue<std::pair<sf::Packet, std::chrono::milliseconds>> _packets;
        unsigned _maxReceivedTick;

        class PacketHandler
        {
          public:
            PacketHandler() = delete;

            ~PacketHandler() = default;

            static void handle(aecs::World &world, sf::Packet &packet)
            {
                // // parse the packet, getting the different ticks / entities / components modified
                // // TODO
                // // ...

                // // rollback to the tick before the packet
                // int tick = 0; // TODO: get the tick before the packet
                // const auto &gameState = world.getGameState(tick - 1);

                // // Apply mofiications to the entities
                // // ...

                // // update the tick & state
                // unsigned currentTick = world.getTick();
                // world.setTick(tick);
                // world.setGameState(gameState);

                // // re-do every tick after the packet with its inputs
                // for (int i = tick; i < currentTick; i++) {
                //     world.setInputs(world.getInputs(i));
                //     world.update();
                // }

                // // normally, here, the game tick is the same as before
                // assert(world.getTick() == currentTick); // TODO: remove in not debug
            }

          private:
        };
    };

} // namespace rtype

#endif // R_TYPE_CLIENTSERVERDATAHANDLERSYSTEM_HPP
