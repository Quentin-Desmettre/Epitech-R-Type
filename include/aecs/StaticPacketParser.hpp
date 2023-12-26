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
        struct PacketHeader {
            sf::Uint16 size;
            sf::Uint8 type;
        };
        struct SystemData {
            aecs::World &world;
            std::size_t clientId;
            std::map<std::size_t, aecs::EntityPtr> &_entitiesMap;
        };
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

        static ParsedData parsePacket(sf::Packet &packet, SystemData &data)
        {
            if (!isPacketValid(packet))
                return {.type = NONE};

            PacketHeader header = {.size = 0, .type = NONE};
            packet >> header.size;
            packet >> header.type;

            if (header.type == NONE)
                return {.type = NONE};

            switch (header.type) {
            case CONNECTED:
                return parseConnected(packet, data, header);
            case GAME_CHANGES:
                return parseGameChanges(packet, data, header);
            case SERVER_PONG:
                return parseServerPong(packet, data, header);
            case GAME_INPUT:
                return parseGameInput(packet, data, header);
            case PING:
                return parsePing(packet, data, header);
            case CLIENT_PONG:
                return parseClientPong(packet, data, header);
            default:
                return {.type = NONE};
            }
        }

      protected:
      private:
        static ParsedData parseConnected(sf::Packet &packet, SystemData &data, PacketHeader &header)
        {
            std::uint8_t clientId;
            packet >> clientId;

            aecs::World::EncodedGameState state = parseGameState(packet);
            return {
                .type = CONNECTED,
                .entityChanges = {state},
                .clientId = clientId,
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

        static ParsedData parseGameChanges(sf::Packet &packet, SystemData &data, PacketHeader &header)
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
            return parsedData;
        }

        static ParsedData parseServerPong(sf::Packet &packet, SystemData &data, PacketHeader &header)
        {
            std::cout << "Pong" << std::endl;
            return {
                .type = SERVER_PONG,
            };
        }

        static ParsedData parseGameInput(sf::Packet &packet, SystemData &data, PacketHeader &header)
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
            return {.type = GAME_INPUT, .inputs = {{data.clientId, inputs}}};
        }

        static ParsedData parsePing(sf::Packet &packet, SystemData &data, PacketHeader &header)
        {
            std::cout << "Ping" << std::endl;
            return {
                .type = PING,
            };
        }

        static ParsedData parseClientPong(sf::Packet &packet, SystemData &data, PacketHeader &header)
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
