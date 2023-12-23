//
// Created by edo on 23/12/23.
//

#include "shared/PacketBuilder.hpp"
#include <cstring>

PacketBuilder &PacketBuilder::operator<<(const std::string &str)
{
    data += reinterpret_cast<const char *>(str.size(), sizeof(int));
    data += str;
    return *this;
}

PacketBuilder &PacketBuilder::operator<<(const char *str)
{
    data += reinterpret_cast<const char *>(strlen(str), sizeof(int));
    data += str;
    return *this;
}

PacketBuilder &PacketBuilder::operator<<(const int &i)
{
    data += reinterpret_cast<const char *>(i, sizeof(int));
    return *this;
}

PacketBuilder &PacketBuilder::operator<<(const float &f)
{
    data += reinterpret_cast<const char *>(f, sizeof(float));
    return *this;
}

PacketBuilder &PacketBuilder::operator<<(const double &d)
{
    data += reinterpret_cast<const char *>(d, sizeof(double));
    return *this;
}

PacketBuilder &PacketBuilder::operator<<(const bool &b)
{
    data += reinterpret_cast<const char *>(b, sizeof(bool));
    return *this;
}

PacketBuilder &PacketBuilder::operator<<(const char &c)
{
    data += c;
    return *this;
}

PacketBuilder &PacketBuilder::operator<<(const sf::Vector2f &v)
{
    return *this << v.x << v.y;
}

PacketBuilder &PacketBuilder::operator>>(std::string &str)
{
    int size = 0;
    std::memcpy(&size, data.c_str(), sizeof(int));
    str = data.substr(sizeof(int), size);
    data = data.substr(sizeof(int) + size);
    return *this;
}

PacketBuilder &PacketBuilder::operator>>(char *str)
{
    int size = 0;
    std::memcpy(&size, data.c_str(), sizeof(int));
    std::memcpy(str, data.c_str() + sizeof(int), size);
    str[size] = '\0';
    data = data.substr(sizeof(int) + size);
    return *this;
}

PacketBuilder &PacketBuilder::operator>>(int &i)
{
    std::memcpy(&i, data.c_str(), sizeof(int));
    data = data.substr(sizeof(int));
    return *this;
}

PacketBuilder &PacketBuilder::operator>>(float &f)
{
    std::memcpy(&f, data.c_str(), sizeof(float));
    data = data.substr(sizeof(float));
    return *this;
}

PacketBuilder &PacketBuilder::operator>>(double &d)
{
    std::memcpy(&d, data.c_str(), sizeof(double));
    data = data.substr(sizeof(double));
    return *this;
}

PacketBuilder &PacketBuilder::operator>>(bool &b)
{
    std::memcpy(&b, data.c_str(), sizeof(bool));
    data = data.substr(sizeof(bool));
    return *this;
}

PacketBuilder &PacketBuilder::operator>>(char &c)
{
    c = data[0];
    data = data.substr(1);
    return *this;
}

PacketBuilder &PacketBuilder::operator>>(sf::Vector2f &v)
{
    return *this >> v.x >> v.y;
}

std::string PacketBuilder::getData() const
{
    return std::to_string(data.size()) + data;
}

void PacketBuilder::clear()
{
    data.clear();
}
