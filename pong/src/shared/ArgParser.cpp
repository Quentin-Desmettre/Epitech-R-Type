/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** ArgParser.cpp
*/

#include "shared/ArgParser.hpp"

ArgParser::ArgParser(int ac, char **av)
{
    for (int i = 0; i < ac; i++) {
        std::string arg = av[i];
        if (arg == "-h" || arg == "--help")
            _help = true;
        if (i + 1 >= ac)
            break;
        if (arg == "--ip")
            _ip = av[i + 1];
        if (arg == "--port")
            _port = av[i + 1];
    }
}

[[nodiscard]] bool ArgParser::getHelp() const
{
    return _help;
}

[[nodiscard]] std::string ArgParser::getIp() const
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

[[nodiscard]] unsigned short ArgParser::getPort() const
{
    unsigned short port = 4242;
    try {
        port = std::stoi(formatPortArg());
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return port;
}
