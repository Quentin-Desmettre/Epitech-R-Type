/*
** EPITECH PROJECT, 2024
** levelEditor
** File description:
** Editor
*/

#pragma once

#include "Window.hpp"
#include <array>
#include <fstream>
#include <map>

enum Mode {
    ADD,
    DELETE,
};

enum Block : char {
    EMPTY = ' ',
    INVINCIBLE = 'X',
    FRAGILE = 'o',
    STRONG = 'O',
};

#define BLOCK_SIZE 98

typedef std::map<int, Block> Line;

static const std::map<Block, std::string> blockPaths = {
    {INVINCIBLE, "assets/diamond.png"},
    {FRAGILE, "assets/dirt.png"},
    {STRONG, "assets/stone.png"},
};

class Editor
{
  public:
    Editor();
    ~Editor() = default;

    bool loadMap(std::string path);
    void saveMap(std::string path = "");
    void clearMap();

    void switchMode(Mode mode);
    void click(sf::Vector2i pos);

    [[nodiscard]] Mode getMode() const;
    [[nodiscard]] const std::array<Line, 11> &getMap() const;

  protected:
  private:
    bool isBlock(const Block block) const;

    Mode _mode;
    Block _selectedBlock;
    std::array<Line, 11> _map;
};
