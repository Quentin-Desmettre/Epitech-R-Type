//
// Created by qdesmettre on 26/12/23.
//

#ifndef R_TYPE_STATICPACKETBUILDER_HPP
#define R_TYPE_STATICPACKETBUILDER_HPP

#include "SFML/Network.hpp"
#include "StaticPacketParser.hpp"
#include "aecs/World.hpp"
#include "shared/PacketBuilder.hpp"

namespace aecs
{
    class StaticPacketBuilder
    {
      public:
        static sf::Packet buildConnectedPacket(std::uint8_t playerId, const aecs::World &world);
        static sf::Packet buildGameChangesPacket(const std::vector<aecs::World::EncodedGameState> &changes);
        static sf::Packet buildServerPongPacket();
        static sf::Packet buildGameInputPacket(const aecs::ClientInputs &inputs);
        static sf::Packet buildPingPacket();
        static sf::Packet buildClientPongPacket(std::uint32_t tick);

      private:
        static sf::Packet makeHeader(PacketTypes packetType, const PacketBuilder &from);
    };

} // namespace aecs

#endif // R_TYPE_STATICPACKETBUILDER_HPP
