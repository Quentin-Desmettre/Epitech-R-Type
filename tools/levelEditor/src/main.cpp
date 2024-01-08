/*
** EPITECH PROJECT, 2024
** Epitech-R-Type
** File description:
** main
*/

#include "Editor.hpp"
#include <iostream>

void print_usage()
{
    std::cout << "USAGE" << std::endl;
    std::cout << "\t./levelEditor [map]" << std::endl;
    std::cout << "DESCRIPTION" << std::endl;
    std::cout << "\tmap:\tmap to load" << std::endl;
    std::cout << "Use A to add blocks, D to delete blocks and Enter to save the map" << std::endl;
    std::cout << "Switch between blocks by pressing A" << std::endl;
}

int main(int ac, char **av)
{
    Editor editor;
    Window window;

    if (ac == 2 && (av[1] == std::string("-h") || av[1] == std::string("--help"))) {
        print_usage();
        return 0;
    }
    if (ac == 2 && editor.loadMap(av[1]) == false)
        std::cerr << "Error: Failed to load map" << std::endl;
    window.run(editor);
}