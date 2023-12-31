/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** ArgParser.hpp
*/

#ifndef R_TYPE_ARGPARSER_HPP
    #define R_TYPE_ARGPARSER_HPP
    #include "Utils.hpp"
    #include "SFML/Network/IpAddress.hpp"

class ArgParser {
  public:
    ArgParser(int ac, char **av);
    [[nodiscard]] bool getHelp() const;
    [[nodiscard]] sf::IpAddress getIp();
    [[nodiscard]] unsigned short getPort() const;

  private:
    [[nodiscard]] std::string formatIpArg();
    [[nodiscard]] std::string formatPortArg() const;
    bool _help = false;
    std::string _default_ip = "127.0.0.1";
    std::string _ip;
    std::string _port;
};

#endif // R_TYPE_ARGPARSER_HPP