//
// Created by qdesmettre on 20/12/23.
//

#ifndef R_TYPE_SERVERUDPSYSTEM_HPP
#define R_TYPE_SERVERUDPSYSTEM_HPP

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
    class ServerUdpSystem : public aecs::ALogicSystem
    {
      public:
        ServerUdpSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
            ALogicSystem(world, entities, {typeid(ClientAdressComponent)})
        {
            _socket.bind(SERVER_UDP_PORT);
            _socket.setBlocking(false);
        }

        ~ServerUdpSystem() override = default;

        void update(const aecs::UpdateParams &updateParams) override
        {
            recieveInputs();
            // sendCorrections();
        }

      private:
        void recieveInputs()
        {
            sf::Packet packet;
            sf::IpAddress sender;
            unsigned short port;
            sf::Socket::Status status = _socket.receive(packet, sender, port);
            aecs::ClientInputs inputs;
            std::size_t clientId = -1;

            if (status != sf::Socket::Done)
                return;
            for (auto &[id, entity] : _entitiesMap) {
                auto &clientAdress = entity->getComponent<ClientAdressComponent>();
                if (clientAdress.adress == sender.toInteger()) {
                    clientId = id;
                    break;
                }
            }
            if (clientId == -1) {
                std::cout << "Unknown client" << std::endl;
                return;
            }

            StaticPacketParser::SystemData systemData = {
                .world = _world, .clientId = clientId, ._entitiesMap = _entitiesMap};
            if (StaticPacketParser::parsePacket(packet, systemData)) {
                auto client = _entitiesMap.find(clientId);
                if (client == _entitiesMap.end())
                    return;
                std::cout << "client " << clientId << " ping: "
                          << client->second->getComponent<ClientPingComponent>().clock.getElapsedTime().asSeconds()
                          << std::endl;
                client->second->getComponent<ClientPingComponent>().clock.restart();
            } else
                std::cout << "Error parsing packet" << std::endl;
        }

        void sendCorrections()
        {
            for (auto &[_, entity] : _entitiesMap) {
                auto &clientAdress = entity->getComponent<ClientAdressComponent>();
                sf::Packet packet;

                packet << (float)(std::rand() % 400) << (float)(std::rand() % 400);
                sf::Socket::Status status =
                    _socket.send(packet, sf::IpAddress(clientAdress.adress), CLIENT_CORRECTIONS_PORT);

                if (status != sf::Socket::Done)
                    std::cout << "Error sending packet" << std::endl;
            }
        }

        sf::UdpSocket _socket;
    };
} // namespace rtype

#endif // R_TYPE_SERVERUDPSYSTEM_HPP
