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
    class EntityFactory {
      public:
        ~EntityFactory() = default;
        static aecs::Entity &createPlayer(bool main = false);
        static aecs::Entity &createBullet(sf::Vector2f position, sf::Vector2f velocity, bool big = false);
        static aecs::Entity &createEnemy(sf::Vector2f position, sf::Vector2f velocity);
        static aecs::Entity &createBackground(int id, sf::Vector2f speed);
        static void setWorld(aecs::World *world);

        private:
        static aecs::World *_world;
    };
}
#include "aecs/World.hpp"

#endif // R_TYPE_ENTITYFACTORY_HPP
