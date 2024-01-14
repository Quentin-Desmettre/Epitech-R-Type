/*
** EPITECH PROJECT, 2023
** Epitech-R-Type
** File description:
** StaticPacketParser
*/

#include "aecs/StaticPacketParser.hpp"
#include <snappy.h>

namespace aecs
{
    bool StaticPacketParser::isPacketValid(const PacketBuilder &packet)
    {
        const std::size_t packetSize = packet.size();
        std::uint16_t givenSize = reinterpret_cast<const std::uint16_t *>(packet.getData().data())[0] + 2;
        return packetSize == givenSize;
    }

    StaticPacketParser::ParsedData StaticPacketParser::parsePacket(const sf::Packet &sfPack, std::size_t clientId)
    {
        // Base check
        PacketBuilder basePacket(sfPack);
        if (!isPacketValid(basePacket))
            return {.type = NONE};

        // Decompress
        std::string decompressed;
        const std::vector<std::byte> &data = basePacket.getData();
        snappy::Uncompress(reinterpret_cast<const char *>(data.data() + 2), basePacket.size() - 2, &decompressed);

        // Put it in a packet
        PacketBuilder packet;
        packet.add(reinterpret_cast<const std::byte *>(decompressed.data()), decompressed.size());

        // Parse
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

    StaticPacketParser::ParsedData StaticPacketParser::parseConnected(PacketBuilder &packet, std::size_t)
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

    aecs::World::EncodedGameState StaticPacketParser::parseGameState(PacketBuilder &packet)
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

    StaticPacketParser::ParsedData StaticPacketParser::parseGameChanges(PacketBuilder &packet, std::size_t)
    {
        std::uint8_t changeCount;
        packet >> changeCount;

        // Get changes
        ParsedData parsedData = {
            .type = GAME_CHANGES,
        };
        for (std::uint8_t i = 0; i < changeCount && packet; i++) {
            aecs::World::EncodedGameState state = parseGameState(packet);
            parsedData.entityChanges.push_back(state);
        }

        // Sort, such that entity changes are in order of tick increasing
        std::sort(parsedData.entityChanges.begin(), parsedData.entityChanges.end(), [](const auto &a, const auto &b) {
            return a.tick < b.tick;
        });
        //        std::cout << "Received " << parsedData.entityChanges.size() << " changes" << std::endl;
        return parsedData;
    }

    StaticPacketParser::ParsedData StaticPacketParser::parseServerPong(PacketBuilder &, std::size_t)
    {
        std::cout << "Pong" << std::endl;
        return {
            .type = SERVER_PONG,
        };
    }

    StaticPacketParser::ParsedData StaticPacketParser::parseGameInput(PacketBuilder &packet, std::size_t clientId)
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

    StaticPacketParser::ParsedData StaticPacketParser::parsePing(PacketBuilder &, std::size_t)
    {
        std::cout << "Ping" << std::endl;
        return {
            .type = PING,
        };
    }

    StaticPacketParser::ParsedData StaticPacketParser::parseClientPong(PacketBuilder &packet, std::size_t)
    {
        std::uint8_t tick;

        packet >> tick;
        //        std::cout << "Client pong: " << static_cast<int>(tick) << std::endl;
        return {
            .type = CLIENT_PONG,
            .tick = tick,
        };
    }

} // namespace aecs
