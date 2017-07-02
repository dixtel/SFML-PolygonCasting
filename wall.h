#ifndef WALL_H
#define WALL_H

#include <SFML/Graphics.hpp>
#include <string>

class Wall
{
    friend class Collision;

    sf::RectangleShape wall;
    sf::Vector2f       position;
    sf::Vector2f       size;
    sf::Vector2f       pos_A;
    sf::Vector2f       pos_B;
    sf::Vector2f       pos_C;
    sf::Vector2f       pos_D;
    sf::Color          color;
    float              angle;
    std::string        texturePath;

    void Update();
public:
    Wall(sf::Vector2f position, sf::Vector2f size, float angle, std::string texturePath);

    sf::RectangleShape &GetRect();
    sf::Vector2f GetPosition();
    sf::Vector2f GetSize();
    void SetPosition(sf::Vector2f position);
    sf::Vector2f GetPointPosition(const char  point); // A, B, C, D position
};

#endif // WALL_H
