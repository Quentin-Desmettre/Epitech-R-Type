//
// Created by edo on 23/12/23.
//

#ifndef R_TYPE_PACKETBUILDER_HPP
#define R_TYPE_PACKETBUILDER_HPP
#include "SFML/Network/Packet.hpp"
#include <SFML/Graphics.hpp>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#if defined(WIN64) || defined(WIN32) || defined(WINNT)
#define uint std::uint32_t
#define ushort std::uint16_t
#endif

class PacketBuilder
{
    public:
        PacketBuilder() = default;
        explicit PacketBuilder(const sf::Packet &packet);

        ~PacketBuilder() = default;

        void pass(int i);

        PacketBuilder &add(const std::byte *rawPtr, size_t size);
        PacketBuilder &operator<<(const std::vector<std::byte> &newData);
        PacketBuilder &operator<<(const std::string &str);

        // template for integral and floating point types
        template <class T>
        PacketBuilder &operator<<(const T toAdd)
        {
            static_assert(std::is_integral<T>::value || std::is_floating_point<T>::value,
                          "T must be an integral or floating point type");
            auto *rawPtr = reinterpret_cast<const std::byte *>(&toAdd);
            add(rawPtr, sizeof(toAdd));
            return *this;
        }

        PacketBuilder &operator+=(const std::vector<std::byte> &newData);

        std::vector<std::byte> getSub();
        PacketBuilder &operator>>(std::string &str);

        template <class T>
        PacketBuilder &operator>>(T &toGet)
        {
            static_assert(std::is_integral<T>::value || std::is_floating_point<T>::value,
                          "T must be an integral or floating point type");
            std::byte *dt = data.data();
            std::memcpy(&toGet, dt, sizeof(toGet));
            data.erase(data.begin(), data.begin() + sizeof(toGet));
            return *this;
        }

        explicit operator const std::vector<std::byte> &() const;
        explicit operator std::vector<std::byte> &();
        explicit operator bool() const;

        [[nodiscard]] sf::Packet toSfPacket() const;

        [[nodiscard]] const std::vector<std::byte> &getData() const;
        std::vector<std::byte> &getData();
        void clear();
        [[nodiscard]] std::size_t size() const;

    private:
        std::vector<std::byte> data;
};

#endif // R_TYPE_PACKETBUILDER_HPP
