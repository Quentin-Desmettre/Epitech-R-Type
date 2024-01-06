//
// Created by qdesmettre on 04/01/24.
//

#include "MapSystem.hpp"
#include "aecs/World.hpp"
#include "rtype/EntityFactory.hpp"
#include <fstream>
#include <utility>

const std::map<rtype::MapSystem::BlockType, rtype::MapSystem::BlockMetaData> rtype::MapSystem::_blocks = {
    {rtype::MapSystem::SOLID, {"assets/sprites/Monster.png", false, false, 0}},
    {rtype::MapSystem::BREAKABLE_LIGHT, {"assets/sprites/Monster.png", true, true, 1}},
    {rtype::MapSystem::BREAKABLE_MEDIUM, {"assets/sprites/Monster.png", true, true, 2}},
    {rtype::MapSystem::BREAKABLE_HEAVY, {"assets/sprites/Monster.png", true, false, 3}},
};

const std::string rtype::MapSystem::_acceptedCharacters = "#oOX ";

rtype::MapSystem::BlockMetaData::BlockMetaData(std::string texturePath, bool canBeShot, bool canBeHitBySmallBullet,
                                               float health) :
    canBeShot(canBeShot),
    canBeHitBySmallBullet(canBeHitBySmallBullet),
    health(health),
    texturePath(std::move(texturePath))
{
}

rtype::MapSystem::Block::Block(rtype::MapSystem::BlockMetaData metaData, const sf::Vector2f &position) :
    metaData(std::move(metaData)),
    position(position)
{
}

rtype::MapSystem::MapSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities) :
    ALogicSystem(world, entities, {}),
    _occupiedSpace(0)
{
    preloadPatterns();
    loadPatterns(0);
}

// Window size: 1044x640

aecs::EntityChanges rtype::MapSystem::update(aecs::UpdateParams &updateParams)
{
    _occupiedSpace -= std::abs(updateParams.deltaTime * BLOCK_SPEED);
    loadPatterns(0);
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
    return {};
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
    std::filesystem::directory_iterator it("assets/patterns");
    for (auto &file : it) {
        if (!file.is_directory())
            continue;
        preloadPatterns(file.path());
    }
}

void rtype::MapSystem::preloadPatterns(const std::filesystem::path &directoryPath)
{
    std::filesystem::directory_iterator it(directoryPath);
    Difficulty difficulty = std::stoi(directoryPath.filename().string());
    std::vector<Pattern> patterns;

    if (difficulty < 0 || difficulty > 10) {
        std::cerr << "Invalid difficulty: " << difficulty << std::endl;
        return;
    }
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
        patterns.push_back(pattern);
    }
    _patterns[difficulty] = patterns;
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
    std::vector<Block> blocks;
    std::size_t patternWidth = 0;
    for (std::size_t y = 0; y < lines.size(); y++) {
        if (lines[y].size() > patternWidth)
            patternWidth = lines[y].size();
        for (std::size_t x = 0; x < lines[y].size(); x++) {
            auto c = static_cast<BlockType>(lines[y][x]);
            if (c == NONE)
                continue;
            auto &metaData = _blocks.at(c);
            blocks.push_back(Block(metaData, {
                static_cast<float>(x * BLOCK_SIZE),
                static_cast<float>(y * BLOCK_SIZE)
            }));
        }
    }
    return {blocks, patternWidth};
}

const rtype::MapSystem::Pattern &rtype::MapSystem::getRandomPattern(rtype::MapSystem::Difficulty difficulty) const
{
    const auto &patterns = _patterns.at(difficulty);
    return patterns[rand() % patterns.size()];
}

void rtype::MapSystem::loadPatternInWorld(const rtype::MapSystem::Pattern &pattern, float startX)
{
    for (const auto &block : pattern.first) {
        EntityFactory::createBlock({block.position.x + startX, block.position.y}, {BLOCK_SIZE * 0.9999, BLOCK_SIZE * 0.9999}, // WTF, why do I need to do this? (without it, blocks collide with each other)
                                   block.metaData.texturePath, BLOCK_SPEED, block.metaData.canBeShot,
                                   block.metaData.health);
    }
}

void rtype::MapSystem::generatePattern(rtype::MapSystem::Difficulty maxDifficulty)
{
    auto difficulty = static_cast<Difficulty>(rand() % (maxDifficulty + 1));
    auto &pattern = getRandomPattern(difficulty);
    loadPatternInWorld(pattern, _occupiedSpace);
    _occupiedSpace += pattern.second * BLOCK_SIZE;
}

void rtype::MapSystem::loadPatterns(rtype::MapSystem::Difficulty maxDifficulty)
{
    // Generate patterns up until all the space has been filled
    const std::size_t WINDOW_WIDTH = 1088; // +2 for a bit of margin

//    std::cout << "Start, occupied space: " << _occupiedSpace << std::endl;
    while (_occupiedSpace < WINDOW_WIDTH * 1.2) {
        generatePattern(maxDifficulty);
//        generatePattern(maxDifficulty);
//        std::cout << "Occupied space: " << _occupiedSpace << std::endl;
    }
}
