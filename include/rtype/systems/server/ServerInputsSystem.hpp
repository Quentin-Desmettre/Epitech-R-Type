//
// Created by qdesmettre on 20/12/23.
//

#ifndef R_TYPE_SERVERINPUTSSYSTEM_HPP
#define R_TYPE_SERVERINPUTSSYSTEM_HPP

#include "aecs/StaticPacketBuilder.hpp"
#include "aecs/StaticPacketParser.hpp"
#include "aecs/SystemBase.hpp"
#include "aecs/World.hpp"
#include "rtype/NetworkGlobals.hpp"
#include "rtype/components/ClientAdressComponent.hpp"
#include "rtype/components/ClientPingComponent.hpp"
#include "rtype/components/PlayerComponent.hpp"
#include <iostream>
#include <map>

namespace rtype
{
    class ServerInputsSystem : public aecs::ALogicSystem
    {
      public:
        ServerInputsSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities);

        ~ServerInputsSystem() override = default;

        aecs::EntityChanges update(aecs::UpdateParams &updateParams) override;

      private:
        void sendPong(sf::IpAddress &sender);

        sf::UdpSocket _socket;
    };
} // namespace rtype

#endif // R_TYPE_SERVERINPUTSSYSTEM_HPP
