//
// Created by qdesmettre on 20/12/23.
//

#ifndef R_TYPE_CLIENTPINGSYSTEM_HPP
#define R_TYPE_CLIENTPINGSYSTEM_HPP

#include "SFML/Network.hpp"
#include "aecs/StaticPacketParser.hpp"
#include "aecs/SystemBase.hpp"
#include "aecs/World.hpp"
#include "rtype/NetworkGlobals.hpp"
#include "rtype/components/ClientPingComponent.hpp"

namespace rtype
{
    class ClientPingSystem : public aecs::ALogicSystem
    {
      public:
        ClientPingSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities);

        ~ClientPingSystem() override = default;

        aecs::EntityChanges update(aecs::UpdateParams &updateParams) override;

      private:
        sf::UdpSocket _socket;
    };
} // namespace rtype

#endif // R_TYPE_CLIENTPINGSYSTEM_HPP
