//
// Created by edo on 23/12/23.
//

#ifndef R_TYPE_PACKETBUILDER_HPP
#define R_TYPE_PACKETBUILDER_HPP
#include <string>
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <cstring>

class PacketBuilder {
    public:
        PacketBuilder() = default;
        ~PacketBuilder() = default;

        PacketBuilder &add(std::byte *rawPtr, size_t size);
        PacketBuilder &operator<<(const std::vector<std::byte> &newData);
        PacketBuilder &operator<<(const std::string &str);
        PacketBuilder &operator<<(int i);
        PacketBuilder &operator<<(float f);
        PacketBuilder &operator<<(double d);
        PacketBuilder &operator<<(bool b);
        PacketBuilder &operator<<(char c);

        PacketBuilder &operator+=(const std::vector<std::byte> &newData);

        std::vector<std::byte> getSub();
        PacketBuilder &operator>>(std::string &str);
        PacketBuilder &operator>>(int &i);
        PacketBuilder &operator>>(float &f);
        PacketBuilder &operator>>(double &d);
        PacketBuilder &operator>>(bool &b);
        PacketBuilder &operator>>(char &c);

        explicit operator std::vector<std::byte>() const;
        explicit operator bool() const;

        [[nodiscard]] std::vector<std::byte> getData() const;
        void clear();
private:
    std::vector<std::byte> data;

};

#endif // R_TYPE_PACKETBUILDER_HPP
