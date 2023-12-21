//
// Created by qdesmettre on 20/12/23.
//

#ifndef R_TYPE_NEWCONNECTIONSYSTEM_HPP
#define R_TYPE_NEWCONNECTIONSYSTEM_HPP

#include "aecs/SystemBase.hpp"
#include "aecs/World.hpp"
#include <SFML/Network.hpp>
#include <iostream>
#include "rtype/components/MyPlayerComponent.hpp"
#include "rtype/components/PositionComponent.hpp"
#include "rtype/components/ClientAdressComponent.hpp"
#include "rtype/components/ClientPortComponent.hpp"

namespace rtype {
    class NewConnectionSystem: public aecs::ALogicSystem {
    public:
        NewConnectionSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
                ALogicSystem(world, entities, {}),
                _listener()
        {
            _listener.listen(53000);
        }

        ~NewConnectionSystem() override = default;

        // listen for new connections and send game state to new clients
        void update(const std::vector<aecs::RenderInput> &inputs, float deltaTime) override {
            sf::TcpSocket socket;

            socket.setBlocking(false);
            if (_listener.accept(socket) == sf::Socket::Done) {
                std::cout << "New connection from " << socket.getRemoteAddress() << std::endl;
                sf::Packet packet;
                for (auto &[_, entity] : _entitiesMap) {
                    if (entity->hasComponent<MyPlayerComponent>()) {
                        auto &posComponent = entity->getComponent<PositionComponent>();
                        packet << posComponent.x << posComponent.y;
                        break;
                    }
                }
                socket.send(packet);
                aecs::Entity &entity = _world.createEntity();
                entity.addComponent<ClientAdressComponent>(socket.getRemoteAddress().toInteger());
                // TODO: get client port and add it to entity via a ClientPortComponent instead of hardcoding 53001
                entity.addComponent<ClientPortComponent>(53001);
                socket.disconnect();
            }
        }

    private:
        sf::TcpListener _listener;
    };
}


#endif //R_TYPE_NEWCONNECTIONSYSTEM_HPP
