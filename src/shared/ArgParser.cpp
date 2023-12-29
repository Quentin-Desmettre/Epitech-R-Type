/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** ArgParser.cpp
*/

#include "shared/ArgParser.hpp"

ArgParser::ArgParser(int ac, char **av) {
    for (int i = 0; i < ac; i++) {
        if (strcmp(av[i], "-h") == 0)
            _help = true;
        if (strcmp(av[i], "--help") == 0)
            _help = true;
        if (i + 1 >= ac)
            break;
        if (strcmp(av[i], "--ip") == 0)
            _ip = av[i + 1];
        if (strcmp(av[i], "--port") == 0)
            _port = av[i + 1];
    }
}

[[nodiscard]] bool ArgParser::getHelp() const {
    return _help;
}

[[nodiscard]] std::string ArgParser::formatIpArg()
{
    if (_ip.empty())
        return _default_ip;
    if (!Utils::isIp(_ip))
        return _default_ip;
    if (_ip == "localhost")
        return _default_ip;
    return _ip;
}

[[nodiscard]] std::string ArgParser::formatPortArg() const
{
    if (_port.empty())
        return "4242";
    if (!Utils::isValidPort(_port))
        return "4242";
    return _port;
}


[[nodiscard]] sf::IpAddress ArgParser::getIp()
{
    return formatIpArg();
}

[[nodiscard]] unsigned short ArgParser::getPort() const {
    unsigned short port = 4242;
    try {
        port = std::stoi(formatPortArg());
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return port;
}
