//
// Created by edo on 23/12/23.
//

#include "shared/PacketBuilder.hpp"
#include <cstring>

PacketBuilder::PacketBuilder(const sf::Packet &packet)
{
    data = std::vector<std::byte>{reinterpret_cast<const std::byte *>(packet.getData()),
                                  reinterpret_cast<const std::byte *>(packet.getData()) + packet.getDataSize()};
}

PacketBuilder &PacketBuilder::add(const std::byte *rawPtr, size_t size)
{
    data.insert(data.end(), rawPtr, rawPtr + size);
    return *this;
}

PacketBuilder &PacketBuilder::operator<<(const std::vector<std::byte> &newData)
{
    data.clear();
    data = newData;
    return *this;
}

PacketBuilder &PacketBuilder::operator<<(const std::string &str)
{
    *this << static_cast<int>(str.size());
    auto *rawPtr = reinterpret_cast<std::byte *>(const_cast<char *>(str.c_str()));
    add(rawPtr, str.size());
    return *this;
}

PacketBuilder &PacketBuilder::operator>>(std::string &str)
{
    int size;
    *this >> size;
    str.resize(size);
    std::byte *dt = data.data();
    std::memcpy(&str[0], dt, size);
    data.erase(data.begin(), data.begin() + size);
    return *this;
}

const std::vector<std::byte> &PacketBuilder::getData() const
{
    return data;
}

std::vector<std::byte> &PacketBuilder::getData()
{
    return data;
}

void PacketBuilder::clear()
{
    data.clear();
}

PacketBuilder &PacketBuilder::operator+=(const std::vector<std::byte> &newData)
{
    *this << static_cast<ushort>(newData.size());
    data.insert(data.end(), newData.begin(), newData.end());
    return *this;
}

PacketBuilder::operator const std::vector<std::byte> &() const
{
    return data;
}

PacketBuilder::operator std::vector<std::byte> &()
{
    return data;
}

PacketBuilder::operator bool() const
{
    return !data.empty();
}
std::vector<std::byte> PacketBuilder::getSub()
{
    std::vector<std::byte> sub;
    ushort size;
    *this >> size;
    sub.insert(sub.end(), data.begin(), data.begin() + size);
    data.erase(data.begin(), data.begin() + size);
    return sub;
}

void PacketBuilder::pass(int i)
{
    data.erase(data.begin(), data.begin() + i);
}

std::size_t PacketBuilder::size() const
{
    return data.size();
}

sf::Packet PacketBuilder::toSfPacket() const
{
    sf::Packet packet;
    packet.append(data.data(), data.size());
    return packet;
}
