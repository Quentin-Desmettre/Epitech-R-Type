//
// Created by qdesmettre on 20/12/23.
//

#ifndef R_TYPE_SERVERCORRECTIONSSYSTEM_HPP
#define R_TYPE_SERVERCORRECTIONSSYSTEM_HPP

#include "aecs/SystemBase.hpp"
#include "aecs/World.hpp"
#include "rtype/NetworkGlobals.hpp"
#include "rtype/StaticPacketParser.hpp"
#include "rtype/components/NetworkTagComponent.hpp"
#include "rtype/components/PositionComponent.hpp"
#include <iostream>
#include <map>

namespace rtype
{
    class ServerCorrectionsSystem : public aecs::ALogicSystem
    {
      public:
        ServerCorrectionsSystem(aecs::World &world,
                                const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
            ALogicSystem(world, entities, {typeid(NetworkTagComponent)})
        {
            _socket.bind(SERVER_CORRECTIONS_PORT);
            _socket.setBlocking(false);
        }

        ~ServerCorrectionsSystem() override = default;

        void update(const aecs::UpdateParams &updateParams) override
        {
            std::vector<std::byte> data;

            for (auto &[_, entity] : _entitiesMap) {
                auto &networkTag = entity->getComponent<NetworkTagComponent>();
                if (networkTag.active && entity->hasComponent<PositionComponent>()) {
                    auto &position = entity->getComponent<PositionComponent>();
                    std::vector<std::byte> posData = position.encode();

                    data.insert(data.end(), posData.begin(), posData.end());
                }
                networkTag.active = false;
            }
            if (!data.empty())
                sendDataToClients(data);
        }

      private:
        void sendDataToClients(std::vector<std::byte> &data)
        {
            sf::Packet packet;
            sf::Uint16 size = data.size() + 1;

            packet << size << GAME_CHANGES;
            packet.append(data.data(), data.size());
            for (auto &client : _world.getClients()) {
                auto &clientAdress = client->getComponent<ClientAdressComponent>();
                sf::Socket::Status status =
                    _socket.send(packet, sf::IpAddress(clientAdress.adress), CLIENT_CORRECTIONS_PORT);

                if (status != sf::Socket::Done)
                    std::cerr << "Error while sending data to client" << std::endl;
            }
        }

        sf::UdpSocket _socket;
    };
} // namespace rtype

#endif // R_TYPE_SERVERCORRECTIONSSYSTEM_HPP
