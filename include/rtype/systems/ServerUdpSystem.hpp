//
// Created by qdesmettre on 20/12/23.
//

#ifndef R_TYPE_SERVERUDPSYSTEM_HPP
#define R_TYPE_SERVERUDPSYSTEM_HPP

#include "aecs/SystemBase.hpp"
#include "aecs/World.hpp"
#include <SFML/Network.hpp>
#include "rtype/components/MyPlayerComponent.hpp"
#include "rtype/components/PositionComponent.hpp"
#include "rtype/components/ClientAdressComponent.hpp"
#include "rtype/components/ClientPortComponent.hpp"
#include "rtype/NetworkGlobals.hpp"
#include <map>
#include <iostream>

namespace rtype {
    class ServerUdpSystem: public aecs::ALogicSystem {
    public:
        ServerUdpSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
                ALogicSystem(world, entities, {typeid(ClientAdressComponent), typeid(ClientPortComponent)})
        {
            _socket.bind(SERVER_UDP_PORT);
            _socket.setBlocking(false);
        }

        ~ServerUdpSystem() override = default;

        void update(const aecs::UpdateParams &updateParams) override {
            for (auto &[_, entity] : _entitiesMap) {
                auto &clientAdress = entity->getComponent<ClientAdressComponent>();
                auto &clientPort = entity->getComponent<ClientPortComponent>();
                sf::Packet packet;

                packet << (float)(std::rand() % 400) << (float)(std::rand() % 400);
                sf::Socket::Status status = _socket.send(packet, sf::IpAddress(clientAdress.adress), clientPort.port);

                if (status != sf::Socket::Done)
                    std::cout << "Error sending packet" << std::endl;
            }
        }

    private:
        sf::UdpSocket _socket;
    };
}


#endif //R_TYPE_SERVERUDPSYSTEM_HPP
