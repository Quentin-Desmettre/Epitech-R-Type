//
// Created by edo on 12/12/23.
//

#ifndef R_TYPE_ENTITYFACTORY_HPP
#define R_TYPE_ENTITYFACTORY_HPP

#include "Entity.hpp"
#include <SFML/Graphics.hpp>

namespace aecs {
    class World;
}

class EntityFactory {
    public:
        ~EntityFactory() = default;

        static void setWorld(aecs::World *world);
        static aecs::Entity &toPlayer(aecs::Entity &entity);

        static aecs::Entity &createBackground();

    private:
        static aecs::World *_world;
};

#include "World.hpp"

#endif // R_TYPE_ENTITYFACTORY_HPP
