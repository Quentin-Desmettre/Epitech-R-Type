/*
** EPITECH PROJECT, 2023
** Epitech-R-Type
** File description:
** StaticPacketParser
*/

#pragma once

#include "SFML/Network.hpp"
#include "aecs/World.hpp"
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
            aecs::ServerInputs inputs;
            std::uint8_t tick;

            // Client received
            std::vector<aecs::World::EncodedGameState> entityChanges;
            std::uint8_t clientId;
        };

        static bool isPacketValid(sf::Packet packet)
        {
            const std::size_t packetSize = packet.getDataSize();
            std::uint16_t givenSize;

            packet >> givenSize;
            return packetSize == givenSize + 2;
        }

        static ParsedData parsePacket(sf::Packet &packet, std::size_t clientId)
        {
            if (!isPacketValid(packet))
                return {.type = NONE};

            sf::Uint16 size = 0;
            sf::Uint8 type = NONE;

            packet >> size;
            packet >> type;
            if (type == NONE)
                return {.type = NONE};

            switch (type) {
            case CONNECTED:
                return parseConnected(packet, clientId);
            case GAME_CHANGES:
                return parseGameChanges(packet, clientId);
            case SERVER_PONG:
                return parseServerPong(packet, clientId);
            case GAME_INPUT:
                return parseGameInput(packet, clientId);
            case PING:
                return parsePing(packet, clientId);
            case CLIENT_PONG:
                return parseClientPong(packet, clientId);
            default:
                return {.type = NONE};
            }
        }

      protected:
      private:
        static ParsedData parseConnected(sf::Packet &packet, std::size_t clientId)
        {
            std::uint8_t myClientId;
            packet >> myClientId;

            aecs::World::EncodedGameState state = parseGameState(packet);
            return {
                .type = CONNECTED,
                .entityChanges = {state},
                .clientId = myClientId,
            };
        }

        static aecs::World::EncodedGameState parseGameState(sf::Packet &packet)
        {
            unsigned numEntities;
            aecs::World::EncodedGameState state;
            packet >> state.tick;
            packet >> numEntities;

            for (unsigned i = 0; i < numEntities; i++) {
                unsigned entityId;
                unsigned short dataSize;
                packet >> entityId;
                packet >> dataSize;

                // get the next dataSize bytes
                std::vector<std::byte> entityData(dataSize);
                for (unsigned short j = 0; j < dataSize; j++) {
                    std::uint8_t byte;
                    packet >> byte;
                    entityData[j] = *reinterpret_cast<std::byte *>(&byte);
                }

                // Put it in the map
                state.encodedEntities[entityId] = entityData;
            }
            return state;
        }

        static ParsedData parseGameChanges(sf::Packet &packet, std::size_t clientId)
        {
            std::uint8_t changeCount;
            packet >> changeCount;

            // Get changes
            ParsedData parsedData = {
                .type = GAME_CHANGES,
            };
            for (std::uint8_t i = 0; i < changeCount; i++) {
                aecs::World::EncodedGameState state = parseGameState(packet);
                parsedData.entityChanges.push_back(state);
            }

            // Sort, such that entity changes are in order of tick increasing
            std::sort(parsedData.entityChanges.begin(), parsedData.entityChanges.end(),
                      [](const auto &a, const auto &b) {
                          return a.tick < b.tick;
                      });
            std::cout << "Received " << parsedData.entityChanges.size() << " changes" << std::endl;
            return parsedData;
        }

        static ParsedData parseServerPong(sf::Packet &packet, std::size_t clientId)
        {
            std::cout << "Pong" << std::endl;
            return {
                .type = SERVER_PONG,
            };
        }

        static ParsedData parseGameInput(sf::Packet &packet, std::size_t clientId)
        {
            sf::Uint8 nbInputs;
            aecs::ClientInputs inputs;

            packet >> nbInputs;
            inputs.reserve(nbInputs);
            for (sf::Uint8 i = 0; i < nbInputs; i++) {
                sf::Uint8 input;
                packet >> input;
                inputs.emplace_back(input);
            }
            return {.type = GAME_INPUT, .inputs = {{clientId, inputs}}};
        }

        static ParsedData parsePing(sf::Packet &packet, std::size_t clientId)
        {
            std::cout << "Ping" << std::endl;
            return {
                .type = PING,
            };
        }

        static ParsedData parseClientPong(sf::Packet &packet, std::size_t clientId)
        {
            std::uint8_t tick;

            packet >> tick;
            std::cout << "Client pong: " << static_cast<int>(tick) << std::endl;
            return {
                .type = CLIENT_PONG,
                .tick = tick,
            };
        }
    };

} // namespace aecs
