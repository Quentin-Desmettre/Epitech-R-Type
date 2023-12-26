/*
** EPITECH PROJECT, 2023
** Epitech-R-Type
** File description:
** StaticPacketParser
*/

#pragma once

#include "aecs/World.hpp"
#include "rtype/components/MyPlayerComponent.hpp"
#include "rtype/components/PositionComponent.hpp"
#include <SFML/Network.hpp>
#include <arpa/inet.h>
#include <iostream>

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
    struct ParsedPacket {
        PacketTypes type;
        aecs::ClientInputs newInputs;
        std::map<std::size_t, std::vector<std::byte>> entityChanges;
    };

    static bool isPacketValid(sf::Packet &packet)
    {
        const void *data = packet.getData();
        if (!data)
            return false;
        const sf::Uint16 *cast_data = static_cast<const sf::Uint16 *>(data);
        return ntohs(cast_data[0]) == packet.getDataSize() - 2;
    }

    static ParsedPacket parsePacket(sf::Packet &packet)
    {
        if (!isPacketValid(packet))
            return {.type = NONE, .newInputs = {}, .entityChanges = {}};

        sf::Uint16 size = 0;
        sf::Uint8 type = NONE;

        packet >> size;
        packet >> type;
        if (type == NONE)
            return {.type = NONE, .newInputs = {}, .entityChanges = {}};

        switch (type) {
        case CONNECTED:
            return parseConnected(packet);
        case GAME_CHANGES:
            return parseGameChanges(packet);
        case SERVER_PONG:
            return parseServerPong(packet);
        case GAME_INPUT:
            return parseGameInput(packet);
        case PING:
            return parsePing(packet);
        case CLIENT_PONG:
            return parseClientPong(packet);
        default:
            return {.type = NONE, .newInputs = {}, .entityChanges = {}};
        }
    }

  protected:
  private:
    static ParsedPacket parseConnected(sf::Packet &packet)
    {
        return {.type = CONNECTED, .newInputs = {}, .entityChanges = {}};
    }

    static ParsedPacket parseGameChanges(sf::Packet &packet)
    {
        return {.type = GAME_CHANGES, .newInputs = {}, .entityChanges = {}};
    }

    static ParsedPacket parseServerPong(sf::Packet &packet)
    {
        std::cout << "Pong" << std::endl;
        return {.type = SERVER_PONG, .newInputs = {}, .entityChanges = {}};
    }

    static ParsedPacket parseGameInput(sf::Packet &packet)
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
        return {.type = GAME_INPUT, .newInputs = inputs, .entityChanges = {}};
    }

    static ParsedPacket parsePing(sf::Packet &packet)
    {
        std::cout << "Ping" << std::endl;
        return {.type = PING, .newInputs = {}, .entityChanges = {}};
    }

    static ParsedPacket parseClientPong(sf::Packet &packet)
    {
        std::cout << "Pong" << std::endl;
        return {.type = CLIENT_PONG, .newInputs = {}, .entityChanges = {}};
    }
};
