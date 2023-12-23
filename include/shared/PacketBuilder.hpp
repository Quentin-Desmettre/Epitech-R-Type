//
// Created by edo on 23/12/23.
//

#ifndef R_TYPE_PACKETBUILDER_HPP
#define R_TYPE_PACKETBUILDER_HPP
#include <string>
#include <SFML/Graphics.hpp>

class PacketBuilder {
public:
    PacketBuilder() = default;
    ~PacketBuilder() = default;

    PacketBuilder &operator<<(const std::string &str);
    PacketBuilder &operator<<(const char *str);
    PacketBuilder &operator<<(const int &i);
    PacketBuilder &operator<<(const float &f);
    PacketBuilder &operator<<(const double &d);
    PacketBuilder &operator<<(const bool &b);
    PacketBuilder &operator<<(const char &c);
    PacketBuilder &operator<<(const sf::Vector2f &v);

    PacketBuilder &operator>>(std::string &str);
    PacketBuilder &operator>>(char *str);
    PacketBuilder &operator>>(int &i);
    PacketBuilder &operator>>(float &f);
    PacketBuilder &operator>>(double &d);
    PacketBuilder &operator>>(bool &b);
    PacketBuilder &operator>>(char &c);
    PacketBuilder &operator>>(sf::Vector2f &v);

    [[nodiscard]] std::string getData() const;
    void clear();
private:
    std::string data;

};

#endif // R_TYPE_PACKETBUILDER_HPP
