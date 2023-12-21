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
#include <map>
#include <iostream>

namespace rtype {
    class ServerUdpSystem: public aecs::ALogicSystem {
    public:
        ServerUdpSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
                ALogicSystem(world, entities, {typeid(ClientAdressComponent), typeid(ClientPortComponent)})
        {
        }

        ~ServerUdpSystem() override = default;

        void update(const std::vector<aecs::RenderInput> &inputs, float deltaTime) override {
            for (auto &[id, entity] : _entitiesMap) {
                if (_sockets.find(id) == _sockets.end()) {
                    _sockets[id].bind(53000);
                    _sockets[id].setBlocking(false);
                }
            }

            for (auto &[id, socket] : _sockets) {
                auto &clientAdress = _entitiesMap[id]->getComponent<ClientAdressComponent>();
                auto &clientPort = _entitiesMap[id]->getComponent<ClientPortComponent>();
                sf::Packet packet;

                packet << 400.0f << 400.0f;
                sf::Socket::Status status = socket.send(packet, sf::IpAddress(clientAdress.adress), clientPort.port);

                if (status != sf::Socket::Done)
                    std::cout << "Error sending packet" << std::endl;
            }
        }

    private:
        std::map<unsigned int, sf::UdpSocket> _sockets;
    };
}


#endif //R_TYPE_SERVERUDPSYSTEM_HPP
