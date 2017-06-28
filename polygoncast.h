#ifndef POLYGONCAST_H
#define POLYGONCAST_H

#include <SFML/Graphics.hpp>
#include <gameobject.h>
#include "toolkit.h"

#include <vector>

class PolygonCast
{
    float angleView;
    float distanceView;


    std::vector <Wall*> GetWallsOnPlayerView(sf::Vector2f player_pos, float player_dir, std::vector <Wall*> walls);
public:
    PolygonCast(float angleView, float distanceView);

    std::vector <sf::VertexArray> GetView(Player *player, std::vector <Wall*> walls);
};

#endif // POLYGONCAST_H
