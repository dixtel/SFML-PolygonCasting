#ifndef COLLISION_H
#define COLLISION_H

#include "gameobject.h"
#include "toolkit.h"
#include <vector>
#include <iostream>
#include <math.h>
#include <array>

class Collision
{
    sf::Vector2f GetPlayerCollisionPosition(sf::Vector2f old_pos, sf::Vector2f invalid_pos, sf::Vector2f start_pos_obstacle, sf::Vector2f end_pos_obstacle);
public:
    Collision();

    void SetCollision(Player *player, std::vector<Wall> *walls);
};

#endif // COLLISION_H
