//
// Created by qdesmettre on 20/12/23.
//

#ifndef R_TYPE_CLIENTPINGSYSTEM_HPP
#define R_TYPE_CLIENTPINGSYSTEM_HPP

#include "SFML/Network.hpp"
#include "StaticPacketParser.hpp"
#include "SystemBase.hpp"
#include "World.hpp"
#include "NetworkGlobals.hpp"
#include "common/network/components/ClientPingComponent.hpp"

class ClientPingSystem : public aecs::ALogicSystem
{
    public:
        ClientPingSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities);

        ~ClientPingSystem() override = default;

        aecs::EntityChanges update(aecs::UpdateParams &updateParams) override;

    private:
        sf::UdpSocket _socket;
};

#endif // R_TYPE_CLIENTPINGSYSTEM_HPP
