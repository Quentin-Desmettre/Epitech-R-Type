//
// Created by edo on 23/12/23.
//

#include "shared/PacketBuilder.hpp"
#include <cstring>

PacketBuilder &PacketBuilder::add(std::byte *rawPtr, size_t size)
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

PacketBuilder &PacketBuilder::operator<<(int i)
{
    auto *rawPtr = reinterpret_cast<std::byte *>(&i);
    add(rawPtr, sizeof(i));
    return *this;
}

PacketBuilder &PacketBuilder::operator<<(uint i)
{
    auto *rawPtr = reinterpret_cast<std::byte *>(&i);
    add(rawPtr, sizeof(i));
    return *this;
}

PacketBuilder &PacketBuilder::operator<<(unsigned short i)
{
    auto *rawPtr = reinterpret_cast<std::byte *>(&i);
    add(rawPtr, sizeof(i));
    return *this;
}

PacketBuilder &PacketBuilder::operator<<(float f)
{
    auto *rawPtr = reinterpret_cast<std::byte *>(&f);
    add(rawPtr, sizeof(float));
    return *this;
}

PacketBuilder &PacketBuilder::operator<<(double d)
{
    auto *rawPtr = reinterpret_cast<std::byte *>(&d);
    add(rawPtr, sizeof(d));
    return *this;
}

PacketBuilder &PacketBuilder::operator<<(bool b)
{
    auto *rawPtr = reinterpret_cast<std::byte *>(&b);
    add(rawPtr, sizeof(b));
    return *this;
}

PacketBuilder &PacketBuilder::operator<<(char c)
{
    auto *rawPtr = reinterpret_cast<std::byte *>(&c);
    add(rawPtr, sizeof(c));
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

PacketBuilder &PacketBuilder::operator>>(int &i)
{
    std::byte *dt = data.data();
    std::memcpy(&i, dt, sizeof(i));
    data.erase(data.begin(), data.begin() + sizeof(i));
    return *this;
}
PacketBuilder &PacketBuilder::operator>>(uint &i)
{
    std::byte *dt = data.data();
    std::memcpy(&i, dt, sizeof(i));
    data.erase(data.begin(), data.begin() + sizeof(i));
    return *this;
}

PacketBuilder &PacketBuilder::operator>>(unsigned short &i)
{
    std::byte *dt = data.data();
    std::memcpy(&i, dt, sizeof(i));
    data.erase(data.begin(), data.begin() + sizeof(i));
    return *this;
}

PacketBuilder &PacketBuilder::operator>>(float &f)
{
    std::byte *dt = data.data();
    std::memcpy(&f, dt, sizeof(float));
    data.erase(data.begin(), data.begin() + sizeof(float));
    return *this;
}

PacketBuilder &PacketBuilder::operator>>(double &d)
{
    std::byte *dt = data.data();
    std::memcpy(&d, dt, sizeof(d));
    data.erase(data.begin(), data.begin() + sizeof(d));
    return *this;
}

PacketBuilder &PacketBuilder::operator>>(bool &b)
{
    std::byte *dt = data.data();
    std::memcpy(&b, dt, sizeof(b));
    data.erase(data.begin(), data.begin() + sizeof(b));
    return *this;
}

PacketBuilder &PacketBuilder::operator>>(char &c)
{
    std::byte *dt = data.data();
    std::memcpy(&c, dt, sizeof(c));
    data.erase(data.begin(), data.begin() + sizeof(c));
    return *this;
}

std::vector<std::byte> PacketBuilder::getData() const
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

PacketBuilder::operator std::vector<std::byte>() const
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