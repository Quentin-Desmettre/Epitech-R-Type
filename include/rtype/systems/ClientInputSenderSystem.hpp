//
// Created by qdesmettre on 20/12/23.
//

#ifndef R_TYPE_CLIENTINPUTSENDERSYSTEM_HPP
#define R_TYPE_CLIENTINPUTSENDERSYSTEM_HPP

#include "aecs/SystemBase.hpp"
#include "aecs/World.hpp"
#include "rtype/NetworkGlobals.hpp"
#include "rtype/StaticPacketParser.hpp"
#include <SFML/Network.hpp>

namespace rtype
{

    /**
     * @brief Sends inputs to server
     */
    class ClientInputSenderSystem : public aecs::ALogicSystem
    {
      public:
        ClientInputSenderSystem(aecs::World &world,
                                const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
            ALogicSystem(world, entities, {})
        {
            _socket.setBlocking(false);
        }

        ~ClientInputSenderSystem() override = default;

        /*
         * send inputs to server, via UDP
         */
        void update(const aecs::UpdateParams &updateParams) override
        {
            sf::Packet packet;
            aecs::ClientInputs myInputs = MY_INPUTS(updateParams.inputs);
            sf::Uint16 size = myInputs.size() + 2;

            if (size == 2)
                return;

            packet << size << PacketTypes::GAME_INPUT << static_cast<sf::Uint8>(size - 2);
            for (auto &input : myInputs)
                packet << static_cast<sf::Uint8>(input);

            _socket.bind(CLIENT_INPUTS_PORT);
            _socket.send(packet, "127.0.0.1", SERVER_UDP_PORT); // TODO: get from ac/av
        }

      private:
        sf::UdpSocket _socket;
    };
} // namespace rtype

#endif // R_TYPE_CLIENTINPUTSENDERSYSTEM_HPP
