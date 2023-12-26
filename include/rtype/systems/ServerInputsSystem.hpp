//
// Created by qdesmettre on 20/12/23.
//

#ifndef R_TYPE_SERVERINPUTSSYSTEM_HPP
#define R_TYPE_SERVERINPUTSSYSTEM_HPP

#include "aecs/SystemBase.hpp"
#include "aecs/World.hpp"
#include "rtype/NetworkGlobals.hpp"
#include "rtype/StaticPacketParser.hpp"
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
            ALogicSystem(world, entities, {typeid(ClientAdressComponent)})
        {
            _socket.bind(SERVER_INPUTS_PORT);
            _socket.setBlocking(false);
        }

        ~ServerInputsSystem() override = default;

        aecs::EntityChanges update(const aecs::UpdateParams &updateParams) override
        {
            sf::Packet packet;
            sf::IpAddress sender;
            unsigned short port;
            sf::Socket::Status status = _socket.receive(packet, sender, port);

            if (status != sf::Socket::Done)
                return {};

            std::size_t clientId = getClientId(sender);
            if (clientId == -1) {
                std::cerr << "Unknown client" << std::endl;
                return {};
            }

            StaticPacketParser::ParsedPacket parsed = StaticPacketParser::parsePacket(packet);

            switch (parsed.type) {
            case NONE:
                std::cerr << "Error parsing packet" << std::endl;
                break;
            case PING:
                sendPong(sender);
                break;
            case CLIENT_PONG:
                break;
            case GAME_INPUT:
                _world.setClientInputs(clientId, parsed.newInputs);
                break;
            default:
                std::cerr << "Unexpected packet type" << std::endl;
                break;
            }

            auto client = _entitiesMap.find(clientId);
            if (client == _entitiesMap.end())
                return {};

            client->second->getComponent<ClientPingComponent>().clock.restart();
            return {};
        }

      private:
        void sendPong(sf::IpAddress &sender)
        {
            sf::Packet packet;
            packet << static_cast<sf::Uint16>(1) << SERVER_PONG;
            sf::Socket::Status status = _socket.send(packet, sender, CLIENT_CORRECTIONS_PORT);

            if (status != sf::Socket::Done)
                std::cerr << "Error sending packet" << std::endl;
        }

        std::size_t getClientId(const sf::IpAddress &sender)
        {
            for (auto &[id, entity] : _entitiesMap) {
                auto &clientAdress = entity->getComponent<ClientAdressComponent>();
                if (clientAdress.adress == sender.toInteger())
                    return id;
            }
            return -1;
        }

        sf::UdpSocket _socket;
    };
} // namespace rtype

#endif // R_TYPE_SERVERINPUTSSYSTEM_HPP
