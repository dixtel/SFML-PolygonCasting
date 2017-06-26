#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics/CircleShape.hpp>
#include <iostream>

class Player
{
    friend class Render;
    friend class Collision;

    sf::CircleShape player;
    sf::Vector2f    position;
    float           radius;
    sf::Color       color;
    std::string     texturePath;

    sf::Vector2f    force;
    sf::Vector2f    velocity;
    sf::Vector2f    max_velocity;
    sf::Vector2f    acceleration;
    float           friction;

    bool            is_colising;
    bool            right;
    bool            left;
    bool            up;
    bool            down;

public:
    Player(sf::Vector2f position, sf::Vector2f size, std::string texturePath);

    void UpdatePhysics();
    void UpdatePlayer();

    void GoRight(bool enable);
    void GoLeft(bool enable);
    void GoUp(bool enable);
    void GoDown(bool enable);

    sf::Vector2f GetPosition();
    sf::Vector2f GetVelocity();
};

#endif // PLAYER_H
