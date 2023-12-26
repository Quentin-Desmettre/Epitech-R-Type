//
// Created by qdesmettre on 20/12/23.
//

#ifndef R_TYPE_SERVERINPUTSSYSTEM_HPP
#define R_TYPE_SERVERINPUTSSYSTEM_HPP

#include "aecs/StaticPacketParser.hpp"
#include "aecs/SystemBase.hpp"
#include "aecs/World.hpp"
#include "rtype/NetworkGlobals.hpp"
#include "rtype/components/ClientAdressComponent.hpp"
#include "rtype/components/ClientPingComponent.hpp"
#include <iostream>
#include <map>

namespace rtype
{
    class ServerInputsSystem : public aecs::ALogicSystem
    {
      public:
        ServerInputsSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
            ALogicSystem(world, entities,
                         {typeid(ClientAdressComponent), typeid(PlayerComponent), typeid(ClientPingComponent)})
        {
            _socket.bind(SERVER_INPUTS_PORT);
            _socket.setBlocking(false);
        }

        ~ServerInputsSystem() override = default;

        aecs::EntityChanges update(aecs::UpdateParams &updateParams) override
        {
            sf::Packet packet;

            // Receive a packet
            sf::IpAddress sender;
            unsigned short port;
            sf::Socket::Status status = _socket.receive(packet, sender, port);
            if (status != sf::Socket::Done)
                return {};

            // Get player from it
            std::size_t clientId = getClientId(sender);
            if (clientId == -1 || _entitiesMap.find(clientId) == _entitiesMap.end()) {
                std::cerr << "Unknown client \"" << sender.toString() << "\"" << std::endl;
                return {};
            }
            auto client = _entitiesMap[clientId];

            // Parse packet
            aecs::StaticPacketParser::SystemData systemData = {
                .world = _world, .clientId = clientId, ._entitiesMap = _entitiesMap};
            aecs::StaticPacketParser::ParsedData parsed = aecs::StaticPacketParser::parsePacket(packet, systemData);
            if (parsed.type == aecs::NONE) {
                std::cerr << "Error parsing packet" << std::endl;
                return {};
            }

            // Handle GAME_INPUT
            if (parsed.type == aecs::GAME_INPUT)
                updateParams.inputs[clientId] = parsed.inputs[clientId];

            // Handle PING
            if (parsed.type == aecs::PING)
                sendPong(sender);

            // Anyway, reset ping clock
            client->getComponent<ClientPingComponent>().clock.restart();
            return {};
        }

      private:
        void sendPong(sf::IpAddress &sender)
        {
            sf::Packet packet = aecs::StaticPacketBuilder::buildServerPongPacket();
            sf::Socket::Status status = _socket.send(packet, sender, CLIENT_CORRECTIONS_PORT);

            if (status != sf::Socket::Done)
                std::cerr << "Error sending packet" << std::endl;
        }

        std::size_t getClientId(const sf::IpAddress &sender)
        {
            for (auto &[id, entity] : _entitiesMap) {
                auto &clientAdress = entity->getComponent<ClientAdressComponent>();
                if (clientAdress.adress == sender.toInteger())
                    return entity->getComponent<PlayerComponent>().playerId;
            }
            return -1;
        }

        sf::UdpSocket _socket;
    };
} // namespace rtype

#endif // R_TYPE_SERVERINPUTSSYSTEM_HPP
