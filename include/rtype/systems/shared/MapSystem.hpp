//
// Created by qdesmettre on 04/01/24.
//

#ifndef R_TYPE_MAPSYSTEM_HPP
#define R_TYPE_MAPSYSTEM_HPP

#include "aecs/SystemBase.hpp"
#include <SFML/Graphics.hpp>
#include <filesystem>

namespace rtype
{

    /**
     * @brief System that handle the map
     *
     * its role is to randomly create the blocks that compose the map
     */
    class MapSystem : public aecs::ALogicSystem
    {
      public:
        MapSystem(aecs::World &world, const std::map<std::size_t, std::shared_ptr<aecs::Entity>> &entities);
        ~MapSystem() override = default;

        aecs::EntityChanges update(aecs::UpdateParams &updateParams) override;

      private:
        enum BlockType {
            NONE = ' ',
            SOLID = '#',
            BREAKABLE_LIGHT = 'o',
            BREAKABLE_MEDIUM = 'O',
            BREAKABLE_HEAVY = 'X',
        };
        struct BlockMetaData {
            BlockMetaData(std::string texturePath, bool canBeShot, bool canBeHitBySmallBullet, float health);

            bool canBeShot;
            bool canBeHitBySmallBullet;
            float health;
            std::string texturePath;
        };
        struct Block {
            Block(BlockMetaData metaData, const sf::Vector2f &position);

            BlockMetaData metaData;
            sf::Vector2f position;
        };
        typedef int Difficulty;
        typedef std::pair<std::vector<Block>, std::size_t> Pattern;

        /**
         * @brief Load the patterns from the assets/patterns directory
         */
        void preloadPatterns();
        void preloadPatterns(const std::filesystem::path &directoryPath);
        static Pattern parseBlocks(const std::vector<std::string> &lines);

        static constexpr const float BLOCK_SIZE = 58;
        static constexpr const float BLOCK_SPEED = -5;
        [[nodiscard]] const Pattern &getRandomPattern(Difficulty difficulty) const;
        void generatePattern(Difficulty maxDifficulty);
        void loadPatterns(Difficulty maxDifficulty);
        static void loadPatternInWorld(const Pattern &pattern, float startX);
        float _occupiedSpace;

        std::map<Difficulty, std::vector<Pattern>> _patterns;
        static const std::map<BlockType, BlockMetaData> _blocks;
        static const std::string _acceptedCharacters;
    };
} // namespace rtype

#endif // R_TYPE_MAPSYSTEM_HPP
