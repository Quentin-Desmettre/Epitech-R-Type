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
        static aecs::Entity &createPlayer(bool isAI = false);
        static aecs::Entity &toPlayer(aecs::Entity &entity);

        static aecs::Entity &createBall();
        static aecs::Entity &toBall(aecs::Entity &entity);
        static aecs::Entity &createBackground();
        static aecs::Entity &createInputs(int input, std::function<void()> &&onInput);
        static aecs::Entity &createButton(const std::string &text, int fontSize, sf::Color color, int zIndex = 0, sf::Vector2f pos = {0, 0}, std::function<void()> &&onClick = nullptr);
        static aecs::Entity &createScore(bool isAI = false);

    private:
        static aecs::World *_world;
};

#include "World.hpp"

#endif // R_TYPE_ENTITYFACTORY_HPP
