#ifndef COLLISION_H
#define COLLISION_H

#include "gameobject.h"
#include <vector>
#include <iostream>
#include <math.h>
#include <array>

class Collision
{

public:
    Collision();

    void SetCollision(Player *player, std::vector<Wall*> walls);
};

#endif // COLLISION_H
