/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** Utils.hpp
*/
#ifndef R_TYPE_UTILS_HPP
    #define R_TYPE_UTILS_HPP
    #include <regex>
    #include <iostream>
    #include <cstring>

class Utils {
  public:
    [[nodiscard]] static bool isIp(const std::string &str) {
        const std::regex ipRegex(R"((\b\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}\b))");

        return std::regex_match(str, ipRegex);
    }

    [[nodiscard]] static bool isValidPort(const std::string &str) {
        // Regular expression for a valid port number (1-65535)
        const std::regex portRegex(R"(\b(0|([1-9]\d{0,3}))\b|([1-5]\d{4})|([1-9]\d{1,3}\b))");

        return std::regex_match(str, portRegex);
    }
};

#endif // R_TYPE_UTILS_HPP
