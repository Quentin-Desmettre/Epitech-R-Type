/*
** EPITECH PROJECT, 2023
** Epitech-R-Type
** File description:
** StaticPacketParser
*/

#pragma once

#include "aecs/World.hpp"
#include <SFML/Network.hpp>
#include <arpa/inet.h>

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

    static bool isPacketValid(sf::Packet &packet)
    {
        const void *data = packet.getData();
        const sf::Uint16 *cast_data = static_cast<const sf::Uint16 *>(data);
        return ntohs(cast_data[0]) == packet.getDataSize() - 2;
    }

    static bool parsePacket(sf::Packet &packet, SystemData &data)
    {
        if (!isPacketValid(packet))
            return false;

        PacketHeader header = {.size = 0, .type = NONE};
        packet >> header.size;
        packet >> header.type;

        if (header.type == NONE)
            return false;

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
            return false;
        }
    }

  protected:
  private:
    static bool parseConnected(sf::Packet &packet, SystemData &data, PacketHeader &header)
    {
        return true;
    }

    static bool parseGameChanges(sf::Packet &packet, SystemData &data, PacketHeader &header)
    {
        return true;
    }

    static bool parseServerPong(sf::Packet &packet, SystemData &data, PacketHeader &header)
    {
        return true;
    }

    static bool parseGameInput(sf::Packet &packet, SystemData &data, PacketHeader &header)
    {
        sf::Uint8 nbInputs;
        aecs::ClientInputs inputs;

        packet >> nbInputs;
        inputs.reserve(nbInputs);
        for (sf::Uint8 i = 0; i < nbInputs; i++) {
            sf::Uint8 input;
            packet >> input;
            std::cout << "input: " << (int)input << std::endl;
            inputs.emplace_back(input);
        }
        data.world.setClientInputs(data.clientId, inputs);
        return true;
    }

    static bool parsePing(sf::Packet &packet, SystemData &data, PacketHeader &header)
    {
        std::cout << "Ping" << std::endl;
        return true;
    }

    static bool parseClientPong(sf::Packet &packet, SystemData &data, PacketHeader &header)
    {
        return true;
    }
};