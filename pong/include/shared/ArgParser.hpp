/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** ArgParser.hpp
*/

#ifndef R_TYPE_ARGPARSER_HPP
#define R_TYPE_ARGPARSER_HPP
#include "SFML/Network/IpAddress.hpp"
#include "Utils.hpp"
#include "NetworkGlobals.hpp"

class ArgParser
{
    public:
        ArgParser(int ac, char **av);
        [[nodiscard]] bool getHelp() const;
        [[nodiscard]] std::string getIp() const;
        [[nodiscard]] unsigned short getServerPort() const;
        [[nodiscard]] unsigned short getClientPort() const;
        [[nodiscard]] unsigned short getTcpPort() const;

    private:
        [[nodiscard]] std::string formatPortArg(std::string port, std::string defaultPort) const;
        bool _help = false;
        std::string _ip;
        std::string _serverPort;
        std::string _clientPort;
        std::string _tcpPort;
};

#endif // R_TYPE_ARGPARSER_HPP
