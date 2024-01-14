//
// Created by qdesmettre on 04/01/24.
//

#include "rtype/systems/server/MapSystem.hpp"
#include "aecs/World.hpp"
#include "rtype/EntityFactory.hpp"
#include "rtype/components/PositionComponent.hpp"
#include "rtype/components/SpriteComponent.hpp"
#include <fstream>
#include <utility>

const std::map<rtype::MapSystem::BlockType, rtype::BlockComponent> rtype::MapSystem::_blocks = {
    {rtype::MapSystem::SOLID, rtype::BlockComponent{"assets/sprites/tilemap.png", false, false, 0}},
    {rtype::MapSystem::BREAKABLE_LIGHT, rtype::BlockComponent{"assets/sprites/tilemap.png", true, true, 5}},
    {rtype::MapSystem::BREAKABLE_MEDIUM, rtype::BlockComponent{"assets/sprites/tilemap.png", true, true, 50}},
};

const std::string rtype::MapSystem::_acceptedCharacters = "#oO ";

const sf::IntRect rtype::MapSystem::rects[] = {
    // No side used
    {51, 51, 17, 17},

    // Only left
    {34, 51, 17, 17},

    // Only right
    {0, 51, 17, 17},

    // Left and right
    {17, 51, 17, 17},

    // Only top
    {51, 34, 17, 17},

    // Top and left
    {34, 34, 17, 17},

    // Top and right
    {0, 34, 17, 17},

    // Top, left and right
    {17, 34, 17, 17},

    // Only bottom
    {51, 0, 17, 17},

    // Bottom and left
    {34, 0, 17, 17},

    // Bottom and right
    {0, 0, 17, 17},

    // Bottom, left and right
    {17, 0, 17, 17},

    // Top and bottom
    {51, 17, 17, 17},

    // Top, bottom and left
    {34, 17, 17, 17},

    // Top, bottom and right
    {0, 17, 17, 17},

    // All sides used
    {17, 17, 17, 17},

    // Misc
    // Bottom right empty
    {68, 0, 17, 17},

    // Bottom left empty
    {85, 0, 17, 17},

    // Top right empty
    {68, 17, 17, 17},

    // Top left empty
    {85, 17, 17, 17},

    // Breakable block
    {85, 51, 17, 17},

    // Strong breakable block
    {68, 51, 17, 17},
};

rtype::MapSystem::MapSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
    ALogicSystem(world, entities, {typeid(BlockComponent)}),
    _occupiedSpace(0)
{
    preloadPatterns();
}

void rtype::MapSystem::onEntityRemoved(const aecs::EntityPtr &entity)
{
    if (!entity->hasComponent<BlockComponent>())
        return;
    auto &position = entity->getComponent<PositionComponent>();
    _loadedPatterns.erase(
        {static_cast<std::size_t>(position.x / BLOCK_SIZE), static_cast<std::size_t>(position.y / BLOCK_SIZE)});
}

// Window size: 1044x640

aecs::EntityChanges rtype::MapSystem::update(aecs::UpdateParams &params)
{
    // procedure to generate map:
    // - check if enough time has passed since last generation
    // - select a random pattern amongst the available ones
    // - load the pattern

    // Pattern must respect those constraints:
    // - be 11 lines high
    // - each line may only contain those blocks:
    //      - #       for solid blocks
    //      - o       for blocks that can be destroyed with 1 normal bullet
    //      - O       for blocks that needs multiple bullets to be destroyed
    //      - X       for blocks that can only be destroyed with charged bullets
    //      - <space> for empty space

    aecs::EntityChanges changes;
    _occupiedSpace += params.deltaTime * BLOCK_SPEED;
    loadPatterns(changes);
    return changes;
}

void rtype::MapSystem::preloadPatterns()
{
    /*
     * Structure of the assets/patterns directory:
     * - assets/patterns:
     *      - 0:
     *          - <some name 1>.txt
     *          - <some name 2>.txt
     *          - <some name 3>.txt
     *          - <some name 4>.txt
     *      - 1:
     *          - <some name 1>.txt
     *          ...
     *      - ...
     *      - 10:
     *          ...
     *
     * With the name of each folder being the difficulty of the patterns it contains
     */
    if (!std::filesystem::exists("assets/patterns")) {
        std::cerr << "Failed to open directory assets/patterns" << std::endl;
        return;
    }
    preloadPatterns("assets/patterns");
}

void rtype::MapSystem::preloadPatterns(const std::filesystem::path &directoryPath)
{
    std::filesystem::directory_iterator it(directoryPath);

    for (auto &file : it) {

        // Open file
        std::ifstream stream(file.path().string());
        if (!stream.is_open()) {
            std::cerr << "Failed to open file " << file.path() << std::endl;
            continue;
        }

        // Load lines
        std::vector<std::string> lines;
        std::string line;
        while (std::getline(stream, line))
            lines.push_back(line);

        // Parse patterns
        auto pattern = parseBlocks(lines);
        if (pattern.second == 0)
            continue;
        _patterns.push_back(pattern);
    }
}

rtype::MapSystem::Pattern rtype::MapSystem::parseBlocks(const std::vector<std::string> &lines)
{
    // Error handling
    if (lines.size() != 11) {
        std::cerr << "Invalid pattern: must be 11 lines high" << std::endl;
        return {};
    }
    for (const auto &line : lines) {
        if (line.find_first_not_of(_acceptedCharacters) == std::string::npos)
            continue;
        std::cerr << "Invalid pattern: each line may only contain those characters: '" << _acceptedCharacters << "'"
                  << std::endl;
        return {};
    }

    // Parse pattern
    std::map<Position, BlockComponent *> blockMap;
    std::vector<BlockComponent> blocks;
    std::size_t patternWidth = 0;
    for (std::size_t y = 0; y < lines.size(); y++) {
        if (lines[y].size() > patternWidth)
            patternWidth = lines[y].size();
        for (std::size_t x = 0; x < lines[y].size(); x++) {
            auto c = static_cast<BlockType>(lines[y][x]);
            if (c == NONE)
                continue;
            auto &metaData = _blocks.at(c);
            blocks.push_back(BlockComponent(metaData.texturePath, metaData.canBeShot, metaData.canBeHitBySmallBullet,
                                            metaData.health, rects[0],
                                            {static_cast<float>(x * BLOCK_SIZE), static_cast<float>(y * BLOCK_SIZE)}));
        }
    }
    return {blocks, patternWidth};
}

std::uint8_t rtype::MapSystem::getUsedSides(std::size_t x, std::size_t y) const
{
    using UsedSide = BlockComponent::UsedSide;
    std::uint8_t usedSides = UsedSide::NONE;

    if (_loadedPatterns.find({x - 1, y}) != _loadedPatterns.end())
        usedSides |= UsedSide::LEFT;
    if (_loadedPatterns.find({x + 1, y}) != _loadedPatterns.end())
        usedSides |= UsedSide::RIGHT;
    if (_loadedPatterns.find({x, y - 1}) != _loadedPatterns.end())
        usedSides |= UsedSide::TOP;
    if (_loadedPatterns.find({x, y + 1}) != _loadedPatterns.end())
        usedSides |= UsedSide::BOTTOM;
    if (_loadedPatterns.find({x - 1, y - 1}) != _loadedPatterns.end())
        usedSides |= UsedSide::TOP_LEFT;
    if (_loadedPatterns.find({x + 1, y - 1}) != _loadedPatterns.end())
        usedSides |= UsedSide::TOP_RIGHT;
    if (_loadedPatterns.find({x - 1, y + 1}) != _loadedPatterns.end())
        usedSides |= UsedSide::BOTTOM_LEFT;
    if (_loadedPatterns.find({x + 1, y + 1}) != _loadedPatterns.end())
        usedSides |= UsedSide::BOTTOM_RIGHT;
    return usedSides;
}

const rtype::MapSystem::Pattern &rtype::MapSystem::getRandomPattern() const
{
    return _patterns[rand() % _patterns.size()];
}

void rtype::MapSystem::loadPatternInWorld(aecs::EntityChanges &changes, const rtype::MapSystem::Pattern &pattern,
                                          float startX)
{
    using UsedSide = BlockComponent::UsedSide;

    for (const auto &block : pattern.first) {
        _loadedPatterns[{static_cast<std::size_t>((block.position.x + startX) / BLOCK_SIZE),
                         static_cast<std::size_t>(block.position.y / BLOCK_SIZE)}] = block;
    }
    for (const auto &block : pattern.first) {
        auto sides = getUsedSides((block.position.x + startX) / BLOCK_SIZE, block.position.y / BLOCK_SIZE);
        auto rect = rects[sides & 0b1111];
        int numDiag = bool(sides & UsedSide::TOP_LEFT) + bool(sides & UsedSide::TOP_RIGHT) +
                      bool(sides & UsedSide::BOTTOM_LEFT) + bool(sides & UsedSide::BOTTOM_RIGHT);
        if (numDiag == 3 && (sides & 0b1111) == 15) {
            if (!(sides & UsedSide::BOTTOM_RIGHT))
                rect = rects[16];
            else if (!(sides & UsedSide::BOTTOM_LEFT))
                rect = rects[17];
            else if (!(sides & UsedSide::TOP_RIGHT))
                rect = rects[18];
            else if (!(sides & UsedSide::TOP_LEFT))
                rect = rects[19];
        }
        if (block.health == 5)
            rect = rects[20];
        else if (block.health == 50)
            rect = rects[21];
        changes.editedEntities.insert(EntityFactory::createBlock({block.position.x + startX, block.position.y},
                                                                 block.texturePath, block.canBeShot, block.health, rect)
                                          .getId());
    }
}

void rtype::MapSystem::generatePattern(aecs::EntityChanges &changes)
{
    auto &pattern = getRandomPattern();
    std::cout << "putting pattern starting at " << _occupiedSpace << std::endl;
    loadPatternInWorld(changes, pattern, _occupiedSpace);
    _occupiedSpace += pattern.second * BLOCK_SIZE;
}

void rtype::MapSystem::loadPatterns(aecs::EntityChanges &changes)
{
    // Generate patterns up until all the space has been filled
    const std::size_t WINDOW_WIDTH = 1088;

    if (_patterns.empty())
        return;
    std::cout << "occupied space: " << _occupiedSpace << std::endl;
    while (_occupiedSpace < WINDOW_WIDTH * 1.2) {
        generatePattern(changes);
    }
}
