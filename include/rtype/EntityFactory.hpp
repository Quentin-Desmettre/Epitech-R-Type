//
// Created by edo on 12/12/23.
//

#ifndef R_TYPE_ENTITYFACTORY_HPP
#define R_TYPE_ENTITYFACTORY_HPP
#include "aecs/Entity.hpp"
#include <SFML/Graphics.hpp>
#include <functional>
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
        static aecs::Entity &createPlayer();
        static aecs::Entity &toPlayer(aecs::Entity &entity);
        static aecs::Entity &createBullet(sf::Vector2f position, sf::Vector2f velocity, int team = 0, bool big = false);
        static aecs::Entity &toBullet(aecs::Entity &entity);
        static aecs::Entity &createEnemy(sf::Vector2f position, sf::Vector2f velocity, bool lil = false);
        static aecs::Entity &toSnake(aecs::Entity &entity);
        static void createSnake(sf::Vector2f position, int nb);
        static aecs::Entity &toEnemy(aecs::Entity &entity);
        static aecs::Entity &createBackground(int id, sf::Vector2f speed);
        static aecs::Entity &createPower(sf::Vector2f position, bool isPowerUp);
        static aecs::Entity &createInputs(int input, std::function<void()> &&onInput);
        static aecs::Entity &toPower(aecs::Entity &entity, bool isPowerUp);
        static void setWorld(aecs::World *world);
        static aecs::Entity &toBlock(aecs::Entity &entity);
        static aecs::Entity &createBlock(sf::Vector2f position, const std::string &texture, bool breakable = false,
                                         float hp = FLOAT_MAX);

      private:
        static aecs::World *_world;
    };
} // namespace rtype
#include "aecs/World.hpp"

#endif // R_TYPE_ENTITYFACTORY_HPP
