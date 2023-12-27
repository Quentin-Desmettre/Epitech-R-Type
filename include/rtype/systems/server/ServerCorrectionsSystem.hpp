//
// Created by qdesmettre on 20/12/23.
//

#ifndef R_TYPE_SERVERCORRECTIONSSYSTEM_HPP
#define R_TYPE_SERVERCORRECTIONSSYSTEM_HPP

#include "aecs/SystemBase.hpp"
#include "aecs/World.hpp"
#include <SFML/Network.hpp>

namespace rtype
{
    class ServerCorrectionsSystem : public aecs::ALogicSystem
    {
      public:
        ServerCorrectionsSystem(aecs::World &world,
                                const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities);

        ~ServerCorrectionsSystem() override = default;

        aecs::EntityChanges update(aecs::UpdateParams &updateParams) override;

      private:
        void sendDataToClients(sf::Packet &data);

        sf::UdpSocket _socket;
    };
} // namespace rtype

#endif // R_TYPE_SERVERCORRECTIONSSYSTEM_HPP
