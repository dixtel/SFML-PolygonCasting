#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics/RectangleShape.hpp>
#include <iostream>

class Player
{
    friend class Render;
    friend class Collision;

    sf::RectangleShape player;
    sf::Vector2f       position;
    sf::Vector2f       size;
    sf::Vector2f       pos_A;
    sf::Vector2f       pos_B;
    sf::Vector2f       pos_C;
    sf::Vector2f       pos_D;
    sf::Color          color;
    std::string        texturePath;

    sf::Vector2f       force;
    sf::Vector2f       velocity;
    sf::Vector2f       max_velocity;
    sf::Vector2f       acceleration;
    float              friction;

    bool               is_colising;
    bool               right;
    bool               left;
    bool               up;
    bool               down;

    void SetPosition(sf::Vector2f pos);
    void UpdatePoints();

public:
    Player(sf::Vector2f position, sf::Vector2f size, std::string texturePath);

    void UpdatePhysics();
    void UpdatePlayer();

    void GoRight(bool enable);
    void GoLeft(bool enable);
    void GoUp(bool enable);
    void GoDown(bool enable);

    sf::Vector2f GetPosition();
    sf::Vector2f GetSize();
    sf::Vector2f GetVelocity();
    sf::Vector2f GetPointPosition(const char  point); // A, B, C, D position

};

#endif // PLAYER_H
