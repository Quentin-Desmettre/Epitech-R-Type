//
// Created by qdesmettre on 20/12/23.
//

#ifndef R_TYPE_SERVERINPUTSSYSTEM_HPP
#define R_TYPE_SERVERINPUTSSYSTEM_HPP

#include "StaticPacketBuilder.hpp"
#include "StaticPacketParser.hpp"
#include "SystemBase.hpp"
#include "World.hpp"
#include "NetworkGlobals.hpp"
#include "common/network/components/ClientAddressComponent.hpp"
#include "ClientPingComponent.hpp"
#include "PlayerComponent.hpp"
#include <iostream>
#include <map>

class ServerInputsSystem : public aecs::ALogicSystem {
    public:
        ServerInputsSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities);

        ~ServerInputsSystem() override = default;

        aecs::EntityChanges update(aecs::UpdateParams &updateParams) override;

    private:
        aecs::EntityPtr &findClient(std::size_t &clientId);

        void handlePacket(aecs::StaticPacketParser::ParsedData &packet, aecs::UpdateParams &updateParams,
                          std::size_t clientId);

        void sendPong(sf::IpAddress &sender);

        sf::UdpSocket _socket;
        sf::IpAddress _sender;
        unsigned short _port;
};

#endif // R_TYPE_SERVERINPUTSSYSTEM_HPP
