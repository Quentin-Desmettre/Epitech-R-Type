/*
** EPITECH PROJECT, 2023
** Epitech-R-Type
** File description:
** StaticPacketParser
*/

#pragma once

#include "World.hpp"
#include <iostream>

namespace aecs
{
    enum PacketTypes : sf::Uint8 {
        NONE = 0x2A,

        CONNECTED = 0x80,
        GAME_CHANGES = 0x81,
        SERVER_PONG = 0x82,

        GAME_INPUT = 0x00,
        PING = 0x01,
        CLIENT_PONG = 0x02,
    };
    class StaticPacketParser
    {
        public:
            struct ParsedData {
                // For both client and server
                PacketTypes type;

                // Server received
                aecs::ServerInputs inputs = {};
                std::uint8_t tick = 0;

                // Client received
                std::vector<aecs::World::EncodedGameState> entityChanges = {};
                std::uint8_t clientId = 0;
            };

            static bool isPacketValid(const PacketBuilder &packet);

            static ParsedData parsePacket(const sf::Packet &sfPack, std::size_t clientId);

        protected:
        private:
            static aecs::World::EncodedGameState parseGameState(PacketBuilder &packet);

            static ParsedData parseConnected(PacketBuilder &packet, std::size_t clientId);
            static ParsedData parseGameChanges(PacketBuilder &packet, std::size_t clientId);
            static ParsedData parseServerPong(PacketBuilder &packet, std::size_t clientId);
            static ParsedData parseGameInput(PacketBuilder &packet, std::size_t clientId);
            static ParsedData parsePing(PacketBuilder &packet, std::size_t clientId);
            static ParsedData parseClientPong(PacketBuilder &packet, std::size_t clientId);
    };

} // namespace aecs
