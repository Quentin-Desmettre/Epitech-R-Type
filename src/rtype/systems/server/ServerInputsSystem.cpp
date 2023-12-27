//
// Created by qdesmettre on 20/12/23.
//

#include "rtype/systems/server/ServerInputsSystem.hpp"

namespace rtype
{
    ServerInputsSystem::ServerInputsSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
            ALogicSystem(world, entities,
                         {typeid(ClientAdressComponent), typeid(PlayerComponent), typeid(ClientPingComponent)})
        {
            _socket.bind(SERVER_INPUTS_PORT);
            _socket.setBlocking(false);
        }

        aecs::EntityChanges ServerInputsSystem::update(aecs::UpdateParams &updateParams)
        {
            sf::Packet packet;

            // Receive a packet
            sf::IpAddress sender;
            unsigned short port;
            sf::Socket::Status status = _socket.receive(packet, sender, port);
            if (status != sf::Socket::Done)
                return {};

            // Get player from it
            std::size_t clientId = -1;
            aecs::EntityPtr &client = _entitiesMap.begin()->second;
            for (auto &[id, entity] : _entitiesMap) {
                auto &clientAdress = entity->getComponent<ClientAdressComponent>();
                if (clientAdress.adress == sender.toInteger()) {
                    clientId = entity->getComponent<PlayerComponent>().playerId;
                    client = entity;
                    break;
                }
            }
            if (clientId == (std::size_t)-1) {
                std::cerr << "Unknown client \"" << sender.toString() << "\"" << std::endl;
                return {};
            }

            aecs::StaticPacketParser::ParsedData parsed = aecs::StaticPacketParser::parsePacket(packet, clientId);

            switch (parsed.type) {
            case aecs::NONE:
                std::cerr << "Error parsing packet" << std::endl;
                break;
            case aecs::PING:
                sendPong(sender);
                break;
            case aecs::CLIENT_PONG:
                break;
            case aecs::GAME_INPUT:
                updateParams.inputs[clientId] = parsed.inputs[clientId];
                break;
            default:
                std::cerr << "Unexpected packet type" << std::endl;
                break;
            }

            // Anyway, reset ping clock
            client->getComponent<ClientPingComponent>().clock.restart();
            return {};
        }

        void ServerInputsSystem::sendPong(sf::IpAddress &sender)
        {
            sf::Packet packet = aecs::StaticPacketBuilder::buildServerPongPacket();
            sf::Socket::Status status = _socket.send(packet, sender, CLIENT_CORRECTIONS_PORT);

            if (status != sf::Socket::Done)
                std::cerr << "Error sending packet" << std::endl;
        }

} // namespace rtype
