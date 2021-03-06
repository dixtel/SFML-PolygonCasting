#ifndef GAME_H
#define GAME_H

#include "render.h"
#include "gameobject.h"
#include "world.h"
#include "collision.h"
#include "polygoncast.h"
#include <string>

class Game
{
    Render      *render;
    GameObject  *gameObject;
    World       *world;
    Collision   *collision;
    PolygonCast *polygonCast;

    const int window_width         = 960;
    const int window_height        = 600;
    const std::string window_title = "PolygonCasting Game";
    const int fps                  = 60;

    bool game_loop                 = true;
    bool pause_loop                = false;
public:
    Game();
    ~Game();

    void StartGameLoop();
};

#endif // GAME_H
