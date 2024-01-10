//
// Created by qdesmettre on 04/01/24.
//

#ifndef R_TYPE_MAPSYSTEM_HPP
#define R_TYPE_MAPSYSTEM_HPP

#include "SFML/Graphics.hpp"
#include "aecs/SystemBase.hpp"
#include "rtype/components/BlockComponent.hpp"
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

        void onEntityRemoved(const aecs::EntityPtr &entity) override;

        static constexpr const float BLOCK_SIZE = 58;
        static constexpr const float BLOCK_SPEED = -5;
        static const sf::IntRect rects[];

      private:
        enum BlockType {
            NONE = ' ',
            SOLID = '#',
            BREAKABLE_LIGHT = 'o',
            BREAKABLE_MEDIUM = 'O',
            BREAKABLE_HEAVY = 'X',
        };
        typedef int Difficulty;
        typedef sf::Vector2<std::size_t> Position;
        typedef std::pair<std::vector<BlockComponent>, std::size_t> Pattern;

        /**
         * @brief Load the patterns from the assets/patterns directory
         */
        void preloadPatterns();
        void preloadPatterns(const std::filesystem::path &directoryPath);
        static Pattern parseBlocks(const std::vector<std::string> &lines);

        [[nodiscard]] const Pattern &getRandomPattern(Difficulty difficulty) const;
        [[nodiscard]] std::uint8_t getUsedSides(std::size_t x, std::size_t y) const;
        void generatePattern(aecs::EntityChanges &changes, Difficulty maxDifficulty);
        aecs::EntityChanges loadPatterns(Difficulty maxDifficulty);
        void loadPatternInWorld(aecs::EntityChanges &changes, const Pattern &pattern, float startX);
        float _occupiedSpace;

        std::map<Difficulty, std::vector<Pattern>> _patterns;

        struct PositionComparator {
            bool operator()(const Position &lhs, const Position &rhs) const {
                return lhs.x < rhs.x || (lhs.x == rhs.x && lhs.y < rhs.y);
            }
        };
        std::map<Position, BlockComponent, PositionComparator> _loadedPatterns;
        static const std::map<BlockType, BlockComponent> _blocks;
        static const std::string _acceptedCharacters;
    };
} // namespace rtype

#endif // R_TYPE_MAPSYSTEM_HPP
