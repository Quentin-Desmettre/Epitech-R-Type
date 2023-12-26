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
        static aecs::Entity &createPlayer(bool main = false, int nb = 0, size_t id = -1);
        static aecs::Entity &createBullet(sf::Vector2f position, sf::Vector2f velocity, int team = 0, bool big = false,
                                          size_t id = -1);
        static aecs::Entity &createEnemy(sf::Vector2f position, sf::Vector2f velocity, bool lil = false,
                                         size_t id = -1);
        static aecs::Entity &createBackground(int id, sf::Vector2f speed);
        static void setWorld(aecs::World *world);

      private:
        static aecs::World *_world;
    };
} // namespace rtype
#include "aecs/World.hpp"

#endif // R_TYPE_ENTITYFACTORY_HPP
