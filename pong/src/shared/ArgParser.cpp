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
        if (arg == "--serverPort")
            _serverPort = av[i + 1];
        if (arg == "--clientPort")
            _clientPort = av[i + 1];
        if (arg == "--tcpPort")
            _tcpPort = av[i + 1];
    }
}

[[nodiscard]] bool ArgParser::getHelp() const
{
    return _help;
}

[[nodiscard]] std::string ArgParser::getIp() const
{
    if (_ip.empty())
        return DEFAULT_IP;
    if (!Utils::isIp(_ip))
        return DEFAULT_IP;
    if (_ip == "localhost")
        return DEFAULT_IP;
    return _ip;
}

[[nodiscard]] std::string ArgParser::formatPortArg(std::string port, std::string defaultPort) const
{
    if (port.empty())
        return defaultPort;
    if (!Utils::isValidPort(port))
        return defaultPort;
    return port;
}

[[nodiscard]] unsigned short ArgParser::getServerPort() const
{
    unsigned short port = SERVER_PORT_NB;
    try {
        port = std::stoi(formatPortArg(_serverPort, SERVER_PORT));
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return port;
}

[[nodiscard]] unsigned short ArgParser::getClientPort() const
{
    unsigned short port = CLIENT_PORT_NB;
    try {
        port = std::stoi(formatPortArg(_clientPort, CLIENT_PORT));
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return port;
}

[[nodiscard]] unsigned short ArgParser::getTcpPort() const
{
    unsigned short port = TCP_PORT_NB;
    try {
        port = std::stoi(formatPortArg(_tcpPort, TCP_PORT));
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return port;
}
