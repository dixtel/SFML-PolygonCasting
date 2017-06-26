#ifndef GAME_H
#define GAME_H

#include "render.h"
#include "gameobject.h"
#include "world.h"
#include "collision.h"
#include <string>

class Game
{
    Render     *render;
    GameObject *gameObject;
    World      *world;
    Collision  *collision;

    const int window_width         = 600;
    const int window_height        = 400;
    const std::string window_title = "PolygonCasting Game";
    const int fps                  = 60;

    bool game_loop = true;
    bool pause_loop = false;


public:
    Game();
    ~Game();

    void StartGameLoop();
};

#endif // GAME_H
