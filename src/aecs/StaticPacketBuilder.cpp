//
// Created by qdesmettre on 26/12/23.
//

#include "aecs/StaticPacketBuilder.hpp"

namespace aecs
{
    sf::Packet StaticPacketBuilder::buildConnectedPacket(std::uint8_t playerId, const aecs::World &world)
    {
        auto serialized = world.serialize();
        PacketBuilder content;

        content << playerId;
        content.add(serialized.data(), serialized.size());
        return makeHeader(PacketTypes::CONNECTED, content);
    }

    sf::Packet StaticPacketBuilder::buildGameChangesPacket(const std::vector<aecs::World::EncodedGameState> &changes)
    {
        std::uint8_t nbChanges = changes.size();
        PacketBuilder content;

        content << nbChanges;
        for (const auto &change : changes) {
            content << static_cast<std::uint32_t>(change.tick);
            content << static_cast<std::uint32_t>(change.encodedEntities.size());
            for (const auto &[id, encodedEntity] : change.encodedEntities) {
                content.add(encodedEntity.data(), encodedEntity.size());
            }
        }
        return makeHeader(PacketTypes::GAME_CHANGES, content);
    }

    sf::Packet StaticPacketBuilder::buildServerPongPacket()
    {
        return makeHeader(PacketTypes::SERVER_PONG, PacketBuilder());
    }

    sf::Packet StaticPacketBuilder::buildGameInputPacket(const aecs::ClientInputs &inputs)
    {
        PacketBuilder content;

        content << static_cast<std::uint8_t>(inputs.size());
        for (const auto &input : inputs)
            content << static_cast<std::uint8_t>(input);
        return makeHeader(PacketTypes::GAME_INPUT, content);
    }

    sf::Packet StaticPacketBuilder::buildPingPacket()
    {
        return makeHeader(PacketTypes::PING, PacketBuilder());
    }

    sf::Packet StaticPacketBuilder::buildClientPongPacket(std::uint32_t tick)
    {
        PacketBuilder content;

        content << tick;
        return makeHeader(PacketTypes::CLIENT_PONG, content);
    }

    sf::Packet StaticPacketBuilder::makeHeader(PacketTypes packetType, const PacketBuilder &from)
    {
        PacketBuilder packet;

        packet << static_cast<std::uint16_t>(from.size() + 1); // +1 for the packetType
        packet << static_cast<std::uint8_t>(packetType);
        packet.add(from.getData().data(), from.size());

        return packet.toSfPacket();
    }

} // namespace aecs
