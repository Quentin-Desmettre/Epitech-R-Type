//
// Created by qdesmettre on 20/12/23.
//

#ifndef R_TYPE_CLIENTSERVERDATAHANDLERSYSTEM_HPP
#define R_TYPE_CLIENTSERVERDATAHANDLERSYSTEM_HPP

#include "aecs/World.hpp"
#include "aecs/SystemBase.hpp"
#include <SFML/Network.hpp>
#include <queue>
#include <chrono>
#include <cassert>

namespace rtype {

    // TODO: put it in constructor
    // Delay before doing a packet, in ms
    static const int BUFFER_DELAY = 30;

    /**
     * @brief System that handle the data received from the server
     */
    class ClientServerDataHandlerSystem: public aecs::ALogicSystem {
    public:
        ClientServerDataHandlerSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
            ALogicSystem(world, entities, {}),
            _maxReceivedTick(0)
        {
        }

        ~ClientServerDataHandlerSystem() override = default;

        void update(const std::vector<aecs::RenderInput> &inputs, float deltaTime) override {
            // Receive a packet
            sf::Packet packet;
            sf::IpAddress sender;
            unsigned short port;
            sf::Socket::Status status = _socket.receive(packet, sender, port);

            // If no packet, return
            if (status != sf::Socket::Done)
                return;

            // If packet:
            // - send pong
            // - check if tick has already been checked
            // - push to queue
            // - do packet received X ms in the past

            // Send pong with tick
            unsigned tick = *(unsigned *)packet.getData(); // TODO: change how tick is get
            sf::Packet pongPacket;
            pongPacket << "pong" << tick;
            _socket.send(pongPacket, sender, port);

            // Check if tick has already been checked
            if (tick <= _maxReceivedTick)
                return;

            // Push to queue
            auto now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
            _maxReceivedTick = tick;
            _packets.emplace(
                packet,
                now
            );

            // Do packet received X ms in the past
            if (_packets.empty())
                return;
            auto &[firstPacket, timeReceived] = _packets.front();
            if (now - timeReceived < std::chrono::milliseconds(BUFFER_DELAY))
                return;
            _packets.pop();
            PacketHandler::handle(_world, firstPacket);
        }

    private:
        sf::UdpSocket _socket;
        std::queue<std::pair<sf::Packet, std::chrono::milliseconds>> _packets;
        unsigned _maxReceivedTick;

        class PacketHandler {
        public:
            PacketHandler() = delete;

            ~PacketHandler() = default;

            static void handle(aecs::World &world, sf::Packet &packet) {
                // parse the packet, getting the different ticks / entities / components modified
                // TODO
                // ...

                // rollback to the tick before the packet
                int tick = 0; // TODO: get the tick before the packet
                const auto &gameState = world.getGameState(tick - 1);

                // Apply mofiications to the entities
                // ...

                // update the tick & state
                unsigned currentTick = world.getTick();
                world.setTick(tick);
                world.setGameState(gameState);

                // re-do every tick after the packet with its inputs
                for (int i = tick; i < currentTick; i++) {
                    world.setInputs(world.getInputs(i));
                    world.update();
                }

                // normally, here, the game tick is the same as before
                assert(world.getTick() == currentTick); // TODO: remove in not debug
            }

        private:
        };
    };

}

#endif //R_TYPE_CLIENTSERVERDATAHANDLERSYSTEM_HPP
