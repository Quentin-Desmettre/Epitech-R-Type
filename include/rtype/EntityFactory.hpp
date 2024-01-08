//
// Created by edo on 12/12/23.
//

#ifndef R_TYPE_ENTITYFACTORY_HPP
#define R_TYPE_ENTITYFACTORY_HPP
#include "aecs/Entity.hpp"
#include <SFML/Graphics.hpp>

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
        static aecs::Entity &toEnemy(aecs::Entity &entity);
        static aecs::Entity &createBackground(int id, sf::Vector2f speed);
        static aecs::Entity &createScore();
        static aecs::Entity &createHealth();
        static void setWorld(aecs::World *world);

      private:
        static aecs::World *_world;
    };
} // namespace rtype
#include "aecs/World.hpp"

#endif // R_TYPE_ENTITYFACTORY_HPP
