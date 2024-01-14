//
// Created by edo on 12/12/23.
//

#ifndef R_TYPE_ENTITYFACTORY_HPP
#define R_TYPE_ENTITYFACTORY_HPP
#include "aecs/Entity.hpp"
#include <SFML/Graphics.hpp>
#include <functional>
#include "rtype/systems/server/PowerSystem.hpp"
#include "rtype/components/PowerComponent.hpp"

#define FLOAT_MAX 3.402823466e+38F

namespace aecs
{
    class World;
}

namespace rtype
{
    class EntityFactory
    {
      public:
        ~EntityFactory() = default;
        static void setWorld(aecs::World *world);
        static void setDifficulty(float *difficulty);
        static float getDifficulty();

        static aecs::Entity &createPlayer();
        static aecs::Entity &toPlayer(aecs::Entity &entity);
        static aecs::Entity &createBullet(sf::Vector2f position, sf::Vector2f velocity, int team = 0, bool big = false, std::size_t shotBy = 0);
        static aecs::Entity &toBullet(aecs::Entity &entity);
        static aecs::Entity &createEnemy(sf::Vector2f position, sf::Vector2f velocity, bool lil = false);
        static aecs::Entity &toSnake(aecs::Entity &entity);
        static void createSnake(sf::Vector2f position, int nb);
        static aecs::Entity &toEnemy(aecs::Entity &entity);
        static aecs::Entity &createBackground(int id, sf::Vector2f speed);
        static aecs::Entity &createInputs(int input, std::function<void()> &&onInput);
        static aecs::Entity &createPower(sf::Vector2f position, PowerComponent::PowerType type);
        static aecs::Entity &toPower(aecs::Entity &entity, PowerComponent::PowerType type);
        static aecs::Entity &createButton(const std::string &text, int fontSize, sf::Color color, int zIndex = 0,
                                          sf::Vector2f pos = {0, 0}, std::function<void()> &&onClick = nullptr);
        static aecs::Entity &toBlock(aecs::Entity &entity);
        static aecs::Entity &createBlock(sf::Vector2f position, const std::string &texture, bool breakable = false,
                                         float hp = FLOAT_MAX, sf::IntRect rect = {0, 0, 0, 0});
        static aecs::Entity &toBossEnemy(aecs::Entity &entity);
        static aecs::Entity &createBossEnemy(sf::Vector2f position, sf::Vector2f velocity);

        static aecs::Entity &createDifficulty();
        static aecs::Entity &toDifficulty(aecs::Entity &entity);

      private:
        static aecs::World *_world;
        static float *_difficulty;
    };
} // namespace rtype
#include "aecs/World.hpp"

#endif // R_TYPE_ENTITYFACTORY_HPP
