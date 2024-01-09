/*
** EPITECH PROJECT, 2024
** levelEditor
** File description:
** Editor
*/

#include "Editor.hpp"

Editor::Editor() :
    _mode(ADD),
    _selectedBlock(INVINCIBLE)
{
}

bool Editor::loadMap(std::string path)
{
    std::ifstream file(path);
    std::string line;
    int y = 0;

    if (!file.is_open())
        return false;
    while (std::getline(file, line)) {
        for (std::size_t x = 0; x < line.size(); x++) {
            if (line[x] == '\n')
                break;
            if (!isBlock(static_cast<Block>(line[x]))) {
                clearMap();
                file.close();
                return false;
            }
            _map[y][x] = static_cast<Block>(line[x]);
        }
        y++;
    }
    return true;
}

void Editor::saveMap(std::string path)
{
    if (path.empty())
        path = "pattern.txt";

    std::ofstream file(path);
    if (!file.is_open()) {
        std::cerr << "Error: Failed to open file" << std::endl;
        return;
    }

    for (auto &line : _map) {
        std::vector<std::pair<int, Block>> blocks;
        for (auto &block : line)
            blocks.push_back(block);
        std::sort(blocks.begin(), blocks.end());
        int i = 0;
        for (auto &block : blocks) {
            for (; i++ < block.first;)
                file << EMPTY;
            file << block.second;
        }
        if (&line != &_map.back())
            file << std::endl;
    }
    file.close();
    std::cout << "Saved map to " << path << std::endl;
}

bool Editor::isBlock(const Block block) const
{
    return block == INVINCIBLE || block == FRAGILE || block == STRONG || block == EMPTY;
}

void Editor::clearMap()
{
    for (auto &line : _map)
        for (auto &block : line)
            block.second = EMPTY;
}

void Editor::switchMode(Mode mode)
{
    if (mode == ADD && _mode == ADD) {
        switch (_selectedBlock) {
        case INVINCIBLE:
            _selectedBlock = FRAGILE;
            break;
        case FRAGILE:
            _selectedBlock = STRONG;
            break;
        case STRONG:
            _selectedBlock = INVINCIBLE;
            break;
        default:
            break;
        }
    } else {
        _mode = mode;
        _selectedBlock = INVINCIBLE;
    }
}

void Editor::click(sf::Vector2i pos)
{
    int x = pos.x / BLOCK_SIZE;
    int y = pos.y / BLOCK_SIZE;

    if (x < 0 || y < 0 || y > 10)
        return;
    if (_mode == ADD)
        _map[y][x] = _selectedBlock;
    else
        _map[y][x] = EMPTY;
}

Block Editor::getSelectedBlock() const
{
    return _selectedBlock;
}

Mode Editor::getMode() const
{
    return _mode;
}

const std::array<Line, 11> &Editor::getMap() const
{
    return _map;
}
