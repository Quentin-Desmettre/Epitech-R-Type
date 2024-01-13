//
// Created by qdesmettre on 20/12/23.
//

#ifndef R_TYPE_CLIENTINPUTSENDERSYSTEM_HPP
#define R_TYPE_CLIENTINPUTSENDERSYSTEM_HPP

#include "SFML/Network.hpp"
#include "StaticPacketBuilder.hpp"
#include "StaticPacketParser.hpp"
#include "SystemBase.hpp"
#include "World.hpp"
#include "NetworkGlobals.hpp"
#include "ClientPingComponent.hpp"

/**
 * @brief Sends inputs to server
 */
class ClientInputSenderSystem : public aecs::ALogicSystem {
    public:
        ClientInputSenderSystem(aecs::World &world,
                                const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities);

        ~ClientInputSenderSystem() override = default;

        /*
         * send inputs to server, via UDP
         */
        aecs::EntityChanges update(aecs::UpdateParams &updateParams) override;

    private:
        sf::UdpSocket _socket;
};

#endif // R_TYPE_CLIENTINPUTSENDERSYSTEM_HPP
